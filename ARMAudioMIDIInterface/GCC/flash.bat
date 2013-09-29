SET HEXFILE=%1.hex
call postbuild %1
jflasharm -open%HEXFILE% -auto -startapp -exit
