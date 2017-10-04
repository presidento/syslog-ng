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
#include <stdlib.h>
#include <glib.h>

LogDriver *
generator_source_driver_new(GlobalConfig *cfg)
{
  GeneratorSourceDriver *self = g_new0(GeneratorSourceDriver, 1);

  log_src_driver_init_instance(&self->super, cfg);

  /* self->super.super.super.init = _generator_source_driver_init; */
  /* self->super.super.super.deinit = _generator_source_driver_deinit; */
  /* self->super.super.super.free_fn = _generator_source_driver_free; */

  /* log_source_options_defaults(&self->options->super); */

  return &self->super.super;
}

