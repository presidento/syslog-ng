#include "messages.h"
#include "logsource.h"
#include "generator-source.h"
#include "generator-source-driver.h"

#include <iv.h>

static void
_generator_source_free(LogPipe *s)
{
  GeneratorSource* self = (GeneratorSource *) s;

  log_source_free(&self->super.super);
}

static gboolean
_generator_source_deinit(LogPipe *s)
{
  GeneratorSource* self = (GeneratorSource *) s;

  return TRUE;
}

static gboolean
_generator_source_init(LogPipe *s)
{
  GeneratorSource *self = (GeneratorSource *) s;

  if (!log_source_init(s))
    return FALSE;

  return TRUE;
}

LogSource *
generator_source_new(GeneratorSourceDriver *owner, GeneratorSourceOptions *options)
{
  GeneratorSource *self = g_new0(GeneratorSource, 1);

  log_source_init_instance(&self->super, owner->super.super.super.cfg);
  log_source_set_options(&self->super, &options->super, owner->super.super.id, NULL, FALSE, FALSE,
                         owner->super.super.super.expr_node);

  self->options = options;
  self->super.super.init = _generator_source_init;
  self->super.super.deinit = _generator_source_deinit;
  self->super.super.free_fn = _generator_source_free;

  return &self->super;
}
