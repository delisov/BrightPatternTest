#pragma once

/// <summary>
/// A parameter for the Requests Generator
/// </summary>
const int MIN_MSEC_WAIT_BETWEEN_REQUESTS = 100;

/// <summary>
/// A parameter for the Requests Generator
/// </summary>
const int MAX_MSEC_WAIT_BETWEEN_REQUESTS = 1000;

/// <summary>
/// A parameter for the Requests Generator
/// </summary>
const int REQUESTS_TO_GENERATE = 50;

/// <summary>
/// A parameter for the main loop, timeout for network select
/// </summary>
const int SELECT_MSEC = 1000;

/// <summary>
/// A parameter for the main loop, how often to update the info in the console
/// </summary>
const int PRINT_REFRESH_MSEC = 100;

/// <summary>
/// A parameter for the Request Processor, how long does the long request take
/// </summary>
const int LONG_WORK_MSEC = 3000;

/// <summary>
/// A parameter for the Request Processor, the concurrency level
/// </summary>
const int THREADS_COUNT = 8;

/// <summary>
/// Number of records to keep in Networks Logs Repo
/// </summary>
const int RECORDS_TO_KEEP = 20;