#define TB_IMPL
#include <termbox2.h>
#include <stdbool.h>
#include <locale.h>

#include <logger.h>
#include <constants.h>
#include <utils.h>
#include <stringutils.h>

typedef struct Context {
  string* content;
  string* temp_str;
  int* cursor;
  int* w;
  int* h;
  bool* dirty;
} Context;

void logContext(Context* context) {
  logformat("Context=%p:\n"
            "\tcontent(%p)=\"%s\" of length %zu of cap %zu\n"
            "\ttemp_str(%p)=\"%s\" of length %zu of cap %zu\n"
            "\tcursor=%d\n"
            "\tw=%d\n" 
            "\th=%d\n"
            "\tdirty=%d\n",
    context,
    context->content,
    context->content->buffer, 
    context->content->size, 
    context->content->capacity,
    context->temp_str,
    context->temp_str->buffer, 
    context->temp_str->size, 
    context->temp_str->capacity,
    *context->cursor,
    *context->w,
    *context->h,
    *context->dirty
  );
}

bool on_key(struct tb_event* event, Context context) {
  if (event->key == TB_KEY_CTRL_Q) {
    logformat("Ctrl+Q pressed...");
    return true;
  }

  if (event->key == TB_KEY_ENTER) {
    *(context.temp_str->buffer) = '\n';
    insertString(context.content, context.temp_str, *context.cursor);
    if ( (*context.cursor) < (*context.w) * (*context.h) )
      (*context.cursor)++;
    return false;
  }
  
  logformat("Trying to add char %c at %d of %p", event->ch, *context.cursor, context.content->buffer);
  *(context.temp_str->buffer) = event->ch;
  insertString(context.content, context.temp_str, *context.cursor);

  logformat("Incrementing cursor...");
  (*context.cursor)++;
  logformat("Cursor=%d", (*context.cursor));

  return false;
}

bool on_mouse(struct tb_event* event) {
  return false;
}

bool on_resize(struct tb_event* event, Context context) {
  if (event->w != *context.w || event->h != *context.h) {
    *context.w = event->w;
    *context.h = event->h - event->h * HUD_SCREEN_RATIO;
    *context.dirty = true;
  }
  return false;
}

void render(string* content, int h, int cursor) {
  logformat("Trying to render: %s", content->buffer);
  int x = 0;
  int y = 0;
  for (int i = 0; i < content->size; i++) {
    char ch = content->buffer[i];
    if (ch == '\n') {
      y++;
      x = 0;
      continue;
    }

    if (i == cursor)
      tb_printf(x, y, TB_WHITE, TB_WHITE, " ");
    else
      tb_printf(x, y, TB_WHITE, 0, "%c", ch);
    
    x++;
  }
}

int main(int argc, char* argv[]) {
  struct tb_event ev;

  initLogger("debug.log");

  int err = tb_init();
  if (failsafe(err, "Initializing termbox2...", "SUCCESS", "\tERROR: %s\n", tb_strerror(err)))
    return loggerfail();

  logformat("Hiding cursor...");
  tb_hide_cursor();
  
  logformat("Setting locale...");
  setlocale(LC_CTYPE, "");

  logformat("Getting console sizes...");
  int w = tb_width();
  int baseh = tb_height();
  int h = baseh - baseh*HUD_SCREEN_RATIO;
  bool dirty = true;

  string content = newString("");
  string temp_str = newStringFromChar('A');

  int cursor = 0;

  Context context = (Context) {
    .content = &content,
    .temp_str = &temp_str,
    .cursor = &cursor,
    .dirty = &dirty,
    .w = &w,
    .h = &h,
  };

  while (true) {
    logformat("Polling Event...");
    tb_poll_event(&ev);

    bool breaks = false;

    switch (ev.type) {
      case TB_EVENT_KEY :
        logformat("KeyEvent...");
        breaks = on_key(&ev, context);
        break;
      case TB_EVENT_MOUSE :
        logformat("MouseEvent...");
        breaks = on_mouse(&ev);
        break;
      case TB_EVENT_RESIZE :
        logformat("ResizeEvent...");
        breaks = on_resize(&ev, context);
        break;
    }
    logContext(&context);

    logformat("Rendering...");
    tb_clear();
    render(&content, h, cursor);
    tb_present();
    logformat("Presenting...");

    if (breaks)
      break;
  }

  logformat("Exiting...");
  tb_shutdown();

  logformat("Finalizing Logger...");

  return loggerok();
}
