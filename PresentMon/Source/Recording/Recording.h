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
// The above copyright notice and this permission notice shall be included in
// all
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

#pragma once

#include <string>

#include "ProcessTermination.h"

// Handles process selection for recording
// State of the current Recording
class Recording {
 public:
  Recording();
  ~Recording();

  // The process of the current foreground window is selected for recording
  // its name is saved and the process is registered for termination
  // Returns the process name or "*" if no process was found
  const std::string& Start();
  // Set recording to false and release the processExitHandle
  void Stop();

  bool IsRecording() const { return recording_; }
  const std::string& GetProcessName() const { return processName_; }
  void SetRecordingDirectory(const std::string& dir) { directory_ = dir; }
  void SetRecordAllProcesses(bool recordAll) { recordAllProcesses_ = recordAll; }
  const std::string& GetDirectory() const { return directory_; }
 private:
  // Returns 0 if no process was found for the foreground window
  DWORD GetProcessFromWindow();
  // Searches for a child window of type "Windows.UI.Core.CoreWindow" which is
  // owned by the correct process
  DWORD GetUWPProcess(HWND window);
  DWORD GetProcess(HWND window);
  // Checks if the window class name is "ApplicationFrameWindow" which is the
  // class for all uwp apps
  bool IsUWPWindow(HWND window);

  static const std::string defaultProcessName_;

  std::string directory_;
  std::string processName_;
  DWORD processID_ = 0;
  bool recording_ = false;
  bool recordAllProcesses_ = false;
  ProcessTermination processTermination_;
};