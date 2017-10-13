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

#include "generator-source-options.h"
#include "messages.h"
#include <stdlib.h>
#include <glib.h>

#define GENERATOR_SOURCE_DEFAULT_FREQUENCY 2.0

void
generator_source_options_init(GeneratorSourceOptions *self, GlobalConfig *cfg, const gchar *group_name)
{
  msg_debug("generator_source options init");
  log_source_options_init(&self->super, cfg, group_name);
}

GeneratorSourceOptions *
generator_source_options_new(void)
{
  msg_debug("generator_source options new");
  GeneratorSourceOptions *self = g_new0(GeneratorSourceOptions, 1);
  log_source_options_defaults(&self->super);
  self->freq = GENERATOR_SOURCE_DEFAULT_FREQUENCY;
  return self;
}

void
generator_source_options_free(GeneratorSourceOptions *self)
{
  msg_debug("generator_source options free");
  log_source_options_destroy(&self->super);
  g_free(self);
}

void
generator_source_options_set_template(GeneratorSourceOptions *self, LogTemplate *template)
{
  log_source_options_set_template(&self->super, template);
}
