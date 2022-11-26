# BrightPatternTest
 
## Important decisions:
- Requests are processed with the special `RequestProcessor` class
- `RequestProcessor` is done with a threadpool
- Network::shouldExit() from the original code is omitted as it seems a contradictory to SOLID to have such a method in the Network class
- All classes but one are normal classes except for ThreadInfoRepo which is done as a singleton. I chose singleton over a static class to be able to pass around a pointer to its instance (used in the ConsolePrinter)

## Shortcuts taken:
- The threadpool is not resettable, i.e. once the thread count is set, there is no way to safely change it
- There is no way to stop the app via console commands, just `ctrl+c`
- I chose `while(true)` for the main loop for simplicity (could create some CLI input for the number of requests, but does not seem important in this case)

## How to change the number of threads, the frequency of requests
Check `constants.h`

## Demo:

![](screencast.gif)
