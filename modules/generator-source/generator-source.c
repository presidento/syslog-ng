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
#include "logmsg/logmsg.h"
#include <unistd.h>

#include <iv.h>

LogMessage *
_generator_source_new_log_msg()
{
  gchar buf[32];
  LogMessage *self;

  g_snprintf(buf, sizeof(buf), "%d", (int) getpid());
  self = log_msg_new_empty();
  log_msg_set_value(self, LM_V_PROGRAM, "syslog-ng", 9);
  log_msg_set_value(self, LM_V_PID, buf, -1);
  self->flags |= LF_LOCAL;
  self->flags |= LF_SIMPLE_HOSTNAME;

  return self;
}

static void
_send_generator_source_message(void * p)
{
  msg_debug("generator_source send message");

  GeneratorSource* self = (GeneratorSource *)p;

  if (log_source_free_to_send(&self->super))
    {
      LogMessage *msg;

      msg = _generator_source_new_log_msg();

      GString *formatted_message = g_string_new("ping");
      log_msg_set_value(msg, LM_V_MESSAGE, formatted_message->str, -1);
      g_string_free(formatted_message, TRUE);

      log_source_post(&self->super, msg);
    }
}

static void
_generator_source_update_watch(void* p)
{
  msg_debug("generator_source update watch");
  GeneratorSource* self = (GeneratorSource *)p;

  if (iv_timer_registered(&self->timer))
    {
      iv_timer_unregister(&self->timer);
    }

  iv_validate_now();
  self->timer.expires = iv_now;
  int seconds = self->options->freq;
  self->timer.expires.tv_sec += seconds;
  self->timer.expires.tv_nsec += (self->options->freq - seconds) * 1e9;
  iv_timer_register(&self->timer);
}


static void
_generator_source_timer_expired_cb(void* p)
{
  msg_debug("generator_source timer expired");
  _send_generator_source_message(p);
  _generator_source_update_watch(p);
}

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

  if (iv_timer_registered(&self->timer))
    {
      iv_timer_unregister(&self->timer);
    }

  return TRUE;
}

static gboolean
_generator_source_init(LogPipe *s)
{
  msg_debug("generator_source init");
  GeneratorSource *self = (GeneratorSource *) s;

  if (!log_source_init(s))
    return FALSE;

  IV_TIMER_INIT(&self->timer);
  self->timer.cookie = self;
  self->timer.handler = _generator_source_timer_expired_cb;
  _generator_source_update_watch(self);

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
