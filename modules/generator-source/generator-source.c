/*
 * Copyright (c) 2017 Balabit
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#include "messages.h"
#include "logsource.h"
#include "generator-source.h"
#include "generator-source-driver.h"

#include <iv.h>

static void
_generator_source_free(LogPipe *s)
{
  msg_debug("generator_source free");
  GeneratorSource* self = (GeneratorSource *) s;

  log_source_free(&self->super.super);
}

static gboolean
_generator_source_deinit(LogPipe *s)
{
  msg_debug("generator_source deinit");
  GeneratorSource* self = (GeneratorSource *) s;

  return TRUE;
}

static gboolean
_generator_source_init(LogPipe *s)
{
  msg_debug("generator_source init");
  GeneratorSource *self = (GeneratorSource *) s;

  if (!log_source_init(s))
    return FALSE;

  return TRUE;
}

LogSource *
generator_source_new(GeneratorSourceDriver *owner, GeneratorSourceOptions *options)
{
  msg_debug("generator_source new");
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
