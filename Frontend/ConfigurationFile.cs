﻿//
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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Frontend
{
  public class RecordingOptions
  {
    public int hotkey;
    public int recordTime;
    public bool simple;
    public bool detailed;
    public bool recordAll;

    private const string section = "Recording";

    public RecordingOptions()
    {
      hotkey = 122;
      recordTime = 60;
      simple = true;
      detailed = true;
      recordAll = true;
    }

    public void Save(string path)
    {
      using (var iniFile = new StreamWriter(path, false))
      {
        iniFile.WriteLine("[" + section + "]");
        iniFile.WriteLine("hotkey=" + hotkey);
        iniFile.WriteLine("recordTime=" + recordTime);
        iniFile.WriteLine("simpleRecording=" + Convert.ToInt32(simple));
        iniFile.WriteLine("detailedRecording=" + Convert.ToInt32(detailed));
        iniFile.WriteLine("recordAllProcesses=" + Convert.ToInt32(recordAll));
      }
    }

    public void Load(string path)
    {
      if(ConfigurationFile.Exists())
      {
        hotkey = ConfigurationFile.ReadInt(section, "hotkey", hotkey, path);
        recordTime = ConfigurationFile.ReadInt(section, "recordTime", recordTime, path);
        simple = ConfigurationFile.ReadBool(section, "simpleRecording", path);
        detailed = ConfigurationFile.ReadBool(section, "detailedRecording", path);
        recordAll = ConfigurationFile.ReadBool(section, "recordAllProcesses", path);
      }
    }
  }

  static class ConfigurationFile
  {
    [DllImport("kernel32", CharSet=CharSet.Unicode)]
    static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);
    [DllImport("kernel32", CharSet = CharSet.Ansi)]
    static extern int GetPrivateProfileInt(string lpAppName, string lpKeyName, int nDefault, string lpFileName);
    [DllImport("kernel32")]
    static extern int GetLastError();

    public static string GetPath()
    {
      const string iniFileName = ("\\OCAT\\Config\\settings.ini");
      return System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + iniFileName;
    }

    public static void Save(RecordingOptions recordingOptions)
    {
      string path = GetPath();
      recordingOptions.Save(path);
    }

    public static bool Exists()
    {
      return File.Exists(GetPath());
    }

    public static string ReadString(string section, string key, string path)
    {
      int bufferSize = 256;
      int stringSize = 0;
      var returnValue = new StringBuilder(bufferSize);
      do
      {
        returnValue.Capacity = bufferSize;
        stringSize = GetPrivateProfileString(section, key, "", returnValue, bufferSize, path);
        bufferSize *= 2;
      }
      //loop until capacity is higher than string size including null terminator
      while (returnValue.Capacity - 1 == stringSize);

      return returnValue.ToString();
    }

    public static int ReadInt(string section, string key, int defaultValue, string path)
    {
      return GetPrivateProfileInt(section, key, defaultValue, path);
    }

    public static bool ReadBool(string section, string key, string path)
    {
      int value = GetPrivateProfileInt(section, key, 0, path);
      switch(value)
      {
        case 0:
          return false;
        case 1:
          return true;
        default:
          System.Diagnostics.Debug.Print("Retrieved invalid bool from config, seting to true");
          return true;
      }
    }
  }
}
