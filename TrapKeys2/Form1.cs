//   Copyright 2010 Chris Targett
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.


using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace TrapKeys2
{
    public partial class Form1 : Form
    {
        [DllImport("TaskKeyHook.dll")]
        private static extern bool AreTaskKeysDisabled();
        [DllImport("TaskKeyHook.dll")]
        private static extern bool DisableTaskKeys(bool bDisable, bool bBeep);

        public Form1()
        {
            InitializeComponent();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            DisableTaskKeys(checkBox1.Checked, false);
            checkBox1.Checked = AreTaskKeysDisabled();
        }
    }
}
