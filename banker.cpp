// Brandon Renner - OS Banker Programming Assignment

// Banker Program

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    //--------------------------------------------- Setup ---------------------------------------------

    // open and input data from text file

    // open input.text file for input
    std::ifstream file;
    file.open("input.txt");
    int numget;
    std::string varget;

    // array to hold numbers in input text file
    int numArray[35] = {};

    // index to use for input algorithm
    int fileIndex = 0;
    int arrayIndex = 0;
    int baseIndex = 0;

    // input reading algorithm: every fourth input from the file is a number to be kept in the array

    // note: first two numbers are number of processes/resources which, like total resource instances,
    //       is not in the table, and so can be assumed as constants of 5 and 3, so these indices are not used
    while (!file.eof())
    {
        fileIndex = baseIndex % 4;

        // condition to accept every number
        if ((fileIndex != 0) && (fileIndex % 3) == 0)
        {
            file >> numget;
            numArray[arrayIndex] = numget;
            ++arrayIndex;
        }
        // condition to skip every other string
        else
        {
            file >> varget;
        }
        ++baseIndex;
        ++fileIndex;
    }

    // end file input
    file.close();

    // create data structures(multidimensional/normal arrays) of sizes N(Processes) and M(Resources): Allocation, Max, Available
    int Allocation[5][3];
    int Max[5][3];
    int Available[3];

    // assign values to Allocation structure, with index of first value ment for it
    int AllocIndex = 2;
    for (int row = 0; row < 5; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            Allocation[row][column] = numArray[AllocIndex];
            ++AllocIndex;
        }
    }

    // assign values to Max structure, with index of first value ment for it
    int MaxIndex = 17;
    for (int row = 0; row < 5; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            Max[row][column] = numArray[MaxIndex];
            ++MaxIndex;
        }
    }

    // assign values to Available structure, with index of first value ment for it
    int AvailIndex = 32;
    for (int column = 0; column < 3; ++column)
    {
        Available[column] = numArray[AvailIndex];
        ++AvailIndex;
    }

    // create Need structure, with the use of Allocation and Max
    int Need[5][3];
    for (int row = 0; row < 5; ++row)
    {
        for (int column = 0; column < 3; ++column)
        {
            Need[row][column] = (Max[row][column] - Allocation[row][column]);
        }
    }

    // print out data structures to use in algorithm
    std::cout << "Allocation Matrix" << std::endl;
    for (int row = 0; row < 5; ++row)
    {
        std::cout << "{ ";
        for (int column = 0; column < 3; ++column)
        {
            std::cout << Allocation[row][column] << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Max Matrix" << std::endl;
    for (int row = 0; row < 5; ++row)
    {
        std::cout << "{ ";
        for (int column = 0; column < 3; ++column)
        {
            std::cout << Max[row][column] << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Need Matrix" << std::endl;
    for (int row = 0; row < 5; ++row)
    {
        std::cout << "{ ";
        for (int column = 0; column < 3; ++column)
        {
            std::cout << Need[row][column] << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Available Vector" << std::endl;
    std::cout << "{ ";
    for (int column = 0; column < 3; ++column)
    {
        std::cout << Available[column] << " ";
    }
    std::cout << "}" << std::endl
              << std::endl;

    std::cout << "--------------------------------------" << std::endl
              << std::endl;

    //---------------------------------------------- Banker Algorithm ----------------------------------------------

    std::cout << "Calculating System State..." << std::endl
              << std::endl;

    // array to track the status of each process
    bool finish[5] = {false};

    // array to store safe sequence if found
    int sequence[5] = {};
    int SeqIndex = 0;

    // boolean variables to indicate status of comparison, whether or not a solution has been found
    bool track;
    bool done = false;

    // integer variable, track how many checks have been done, used to exit algorithm if unsafe
    int counter = 0;

    // Banker Algorithm for-loop, quit no matter what if counter reaches 20
    for (int row = 0; counter < 20; ++row)
    {
        // keep checking processes in a circular way
        row = row % 5;

        // only check process if not already found to be finished
        if (!finish[row])
        {

            // compare Need row with the Available vector, track is true if Need can be satisfied
            for (int column = 0; column < 3; ++column)
            {
                if (Need[row][column] <= Available[column])
                {
                    track = true;
                }
                else
                {
                    track = false;
                    break;
                }
            }

            // if Need can be satisfied:
            // Add corresponding Allocation row to available, set finish index to true(done), put this row(process) in the safe sequence
            if (track)
            {
                for (int column = 0; column < 3; ++column)
                {
                    Available[column] = Available[column] + Allocation[row][column];
                }
                finish[row] = true;
                sequence[SeqIndex] = row;
                ++SeqIndex;
            }
        }
        // if the current row(process) is already done, check if all are. if so, done
        else
        {
            for (int i = 0; i < 5; ++i)
            {
                if (finish[i] == true)
                {
                    done = true;
                }
                else
                {
                    done = false;
                    break;
                }
            }
        }
        // if done, exit algorithm altogether
        if (done)
        {
            break;
        }

        // increment counter variable
        ++counter;
    }

    //---------------------------------------------- Print Result ----------------------------------------------

    // if the system was found to be safe(done), print a message saying so, and print the sequence
    if (done)
    {
        std::cout << "System is in a Safe State" << std::endl;
        std::cout << "A Safe Sequence in this System: < ";
        for (int i = 0; i < 5; ++i)
        {
            std::cout << "P" << sequence[i] << " ";
        }
        std::cout << ">" << std::endl;
    }
    // if the system was found to be unsafe, print a message saying so
    else
    {
        std::cout << "System is in an Unsafe State, no Safe Sequence exists" << std::endl
                  << std::endl;
    }
}
