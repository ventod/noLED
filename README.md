# noLED

A PSP plugin that turn off all LEDs and keeps it that way by redirect sceSysconCtrlLED calls to an empty function.

Currently not working:
  - When you wake the PSP from suspend the redirect is lost and is not reapplied (working on it)
