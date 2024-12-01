#!/bin/bash
prog=$0

function usage {
	cat <<HELP_USAGE
    Generate the required files for any previous or current Advent of Code
    competition. If no year or day options are specified, this script defaults
    to the current date.

    Usage: $prog [-h] [-y year] [-d day]
        -h              Display help options
        -y <year>       Year to generate files for
        -d <day>        Day to generate files for
HELP_USAGE
}

# Define variables
year="$(date +%Y)"
day="$(date +%d)"

while [[ $# -gt 0 ]]; do
	opt="$1"
	shift
	case "$opt" in
	"-h")
		usage
		exit 1
		;;
	"-y")
		year=$1
		shift
		;;
	"-d")
		day=$1
		shift
		;;
	*)
		usage
		exit 1
		;;
	esac
done

# Check year/day input formats
if [[ ! $year =~ ^[0-9]{4}$ ]]; then
	echo "ERROR: Year should be in the form '####'"
	exit 1
fi

if [[ ! $day =~ ^[0-9]{2}$ ]]; then
	echo "ERROR: Day should be in the form '##'"
	exit 1
fi

# Check for existing directory
if [[ ! -d "$year/$day" ]]; then
	echo "Directory $year/$day does not exist. Creating..."
	mkdir -p "$year/$day"
fi

# Download the file from the website
echo "Downloading input file for $year/$day..."
curl "https://adventofcode.com/$year/day/$((10#$day))/input" >"$year/$day/input.txt"
