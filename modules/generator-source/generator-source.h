/*
 * Copyright (c) 2017 Balabit
 *
 */
#ifndef GENERATOR_SOURCE_H_INCLUDED
#define GENERATOR_SOURCE_H_INCLUDED

#include "driver.h"
#include "iv.h"
#include "logsource.h"
#include "generator-source-options.h"
#include "generator-source-driver.h"

typedef struct _GeneratorSource GeneratorSource;

struct _GeneratorSource
{
  LogSource super;
  GeneratorSourceOptions *options;
  struct iv_timer timer;
};

LogSource *generator_source_new(GeneratorSourceDriver *owner, GeneratorSourceOptions *options);

#endif // GENERATOR_SOURCE_H_INCLUDED

