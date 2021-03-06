//
// Copyright(c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "Hook.h"
#include "MessageLog.h"

Hook::Hook() {}
Hook::~Hook() { Deactivate(); }
bool Hook::Activate(const HookInfo& info)
{
  if (hook_) return true;

  g_messageLog.Log(MessageLog::LOG_INFO, "Hook", " Activating hook");

  const HMODULE dll = LoadLibrary(info.libName.c_str());
  if (!dll) {
    g_messageLog.Log(MessageLog::LOG_ERROR, "Hook", " DLL not found ", GetLastError());
    return false;
  }

  const HOOKPROC addr = reinterpret_cast<HOOKPROC>(GetProcAddress(dll, info.hookFunction.c_str()));
  if (!addr) {
    g_messageLog.Log(MessageLog::LOG_ERROR, "Hook", " Hooking function not found ", GetLastError());
    return false;
  }

  hook_ = SetWindowsHookEx(info.hookID, addr, dll, info.threadID);
  if (!hook_) {
    g_messageLog.Log(MessageLog::LOG_ERROR, "Hook", " Unable to hook", GetLastError());
    return false;
  }

  g_messageLog.Log(MessageLog::LOG_INFO, "Hook", " activated successfully");
  return true;
}

void Hook::Deactivate()
{
  if (hook_) {
    if (!UnhookWindowsHookEx(hook_)) {
      g_messageLog.Log(MessageLog::LOG_WARNING, "Hook", " Error unhooking", GetLastError());
    }
    hook_ = nullptr;
  }
}