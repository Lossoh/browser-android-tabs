// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "extensions/renderer/get_script_context.h"

#include "base/logging.h"
#include "content/public/renderer/worker_thread.h"
#include "extensions/renderer/script_context.h"
#include "extensions/renderer/script_context_set.h"
#include "extensions/renderer/worker_thread_dispatcher.h"

namespace extensions {

ScriptContext* GetScriptContextFromV8Context(v8::Local<v8::Context> context) {
  ScriptContext* script_context =
      content::WorkerThread::GetCurrentId() > 0
          ? WorkerThreadDispatcher::GetScriptContext()
          : ScriptContextSet::GetContextByV8Context(context);
  DCHECK(!script_context || script_context->v8_context() == context);
  return script_context;
}

ScriptContext* GetScriptContextFromV8ContextChecked(
    v8::Local<v8::Context> context) {
  ScriptContext* script_context = GetScriptContextFromV8Context(context);
  CHECK(script_context);
  return script_context;
}

}  // namespace extensions
