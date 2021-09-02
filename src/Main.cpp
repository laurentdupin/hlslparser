#include "HLSLParser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

std::string ReadFile( const char* fileName )
{
	std::ifstream ifs( fileName );
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}

void PrintUsage()
{
	std::cerr << "usage: hlslparser [-h] FILENAME ENTRYNAME\n"
		<< "\n"
		<< "Output HLSL Parsing results to JSON.\n"
		<< "\n"
		<< "positional arguments:\n"
		<< " FILENAME    input file name\n"
		<< " ENTRYNAME   entry point of the shader\n"
		<< "\n"
		<< "optional arguments:\n"
		<< " -h, --help  show this help message and exit\n";
}

int main( int argc, char* argv[] )
{
	using namespace M4;

	// Parse arguments
	const char* fileName = NULL;
	const char* entryName = NULL;

	for( int argn = 1; argn < argc; ++argn )
	{
		const char* const arg = argv[ argn ];

		if( String_Equal( arg, "-h" ) || String_Equal( arg, "--help" ) )
		{
			PrintUsage();
			return 0;
		}
		else if( fileName == NULL )
		{
			fileName = arg;
		}
		else if( entryName == NULL )
		{
			entryName = arg;
		}
		else
		{
			Log_Error( "Too many arguments\n" );
			PrintUsage();
			return 1;
		}
	}

	if( fileName == NULL || entryName == NULL )
	{
		Log_Error( "Missing arguments\n" );
		PrintUsage();
		return 1;
	}

	if (!std::filesystem::exists(fileName))
	{
		Log_Error("File does not exits\n");
		return 1;
	}

	// Read input file
	const std::string source = ReadFile( fileName );

	// Parse input file
	HLSLParser parser(fileName, source.data(), source.size() );
	HLSLTree tree;

	if( !parser.Parse( &tree ) )
	{
		Log_Error( "Parsing failed, aborting\n" );
		return 1;
	}

	nlohmann::json output = nlohmann::json::array();

	HLSLStatement* nextStatement = tree.GetRoot()->statement;
	while (nextStatement != nullptr)
	{
		output.emplace_back(nextStatement->ConvertToJSON());
		nextStatement = nextStatement->nextStatement;
	}

	FILE* file = NULL;
	std::string strOutput = output.dump(2);

	fopen_s(&file, (std::string(fileName) + ".analysis").c_str(), "w");

	if (file != NULL)
	{
		fprintf(file, "%s", strOutput.c_str());
		fclose(file);
	}
	else
	{
		Log_Error("Failed to output analysis\n");
		return 1;
	}

	return 0;
}
