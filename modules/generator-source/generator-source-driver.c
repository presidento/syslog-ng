/*
 * Copyright (c) 2017 Máté Farkas
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

#include "generator-source-driver.h"
#include "generator-source-options.h"
#include "generator-source.h"
#include <stdlib.h>
#include <glib.h>
#include "messages.h"

static void
_generator_source_driver_free(LogPipe *s)
{
  msg_error("X driver free");

  GeneratorSourceDriver *self = (GeneratorSourceDriver*) s;

  generator_source_options_free(self->options);
  log_src_driver_free(s);
}

static gboolean
_generator_source_driver_deinit(LogPipe *s)
{
  msg_error("X driver deinit", NULL);

  GeneratorSourceDriver *self = (GeneratorSourceDriver*) s;

  if(self->source)
    {
      log_pipe_deinit((LogPipe *)self->source);
      log_pipe_unref((LogPipe *)self->source);
      self->source = NULL;
    }

  if (!log_src_driver_deinit_method(s))
    return FALSE;

  return TRUE;
}

static gboolean
_generator_source_driver_init(LogPipe *s)
{
  msg_error("X driver init", NULL);

  GeneratorSourceDriver *self = (GeneratorSourceDriver*) s;
  GlobalConfig *cfg = log_pipe_get_config(s);

  if (!log_src_driver_init_method(s))
    return FALSE;

  generator_source_options_init(self->options, cfg, self->super.super.group);

  self->source = generator_source_new(self, self->options);
  log_pipe_append(&self->source->super, s);

  if (!log_pipe_init(&self->source->super))
    {
      log_pipe_unref(&self->source->super);
      self->source = NULL;
      return FALSE;
    }

  return TRUE;
}


LogDriver *
generator_source_driver_new(GlobalConfig *cfg)
{
  msg_error("X driver new", NULL);

  GeneratorSourceDriver *self = g_new0(GeneratorSourceDriver, 1);

  log_src_driver_init_instance(&self->super, cfg);

  self->super.super.super.init = _generator_source_driver_init;
  self->super.super.super.deinit = _generator_source_driver_deinit;
  self->super.super.super.free_fn = _generator_source_driver_free;

  self->options = generator_source_options_new();
  log_source_options_defaults(&self->options->super);

  return &self->super.super;
}
