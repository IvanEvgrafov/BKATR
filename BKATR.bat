REM   C:\Program Files\CodeMaster-96\1_32_00\Projects\BKATR\BKATR.ipp - command file
REM   Created: Jan 25, 2023  16:12:18

PushD
CD..
If Exist "C:\Program Files\CodeMaster-96\1_32_00\Projects\BKATR\Debug\Exe\BKATR.MCE" Del "C:\Program Files\CodeMaster-96\1_32_00\Projects\BKATR\Debug\Exe\BKATR.MCE"
If Exist "C:\Program Files\CodeMaster-96\1_32_00\Projects\BKATR\Debug\Exe\BKATR.HEX" Del "C:\Program Files\CodeMaster-96\1_32_00\Projects\BKATR\Debug\Exe\BKATR.HEX"
"C:\Program Files\CodeMaster-96\1_32_00\CMC-96\Bin\MCC96.EXE" @"C:\Program Files\CodeMaster-96\1_32_00\Projects\BKATR\main.{1}"
"C:\Program Files\CodeMaster-96\1_32_00\CMC-96\Bin\MCLINK.EXE" @"C:\Program Files\CodeMaster-96\1_32_00\Projects\BKATR\BKATR.(1)"
PopD
