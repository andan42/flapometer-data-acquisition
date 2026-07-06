#include "flapoRequester.h"
#include "flapoLastDatabaseTime.h"

int getLastDatabaseTime(FlapoRequester *requester, std::string patientUuid)
{
    // Replace with your backend URL
    std::string endpoint = "/patients/" + patientUuid + "/last_timestamp"; // Construct the endpoint path

    // Perform the GET request
    std::string response = requester->get(endpoint);

    // Check if the response is empty, which indicates a failure
    if (response.empty())
    {
        // Handle error case or log as necessary
        return 0; // Return 0 as a fallback if the response is empty or invalid
    }

    try
    {
        // Convert the response string to an integer (timestamp)
        int lastTimestamp = std::stoi(response);
        return lastTimestamp;
    }
    catch (const std::exception &e)
    {
        // Handle conversion error, e.g., if the response is not a valid integer
        // Log error if needed
        return 0; // Return 0 if parsing fails
    }
}