#!/usr/bin/awk -f

BEGIN {
    # Fortran is case insensitive, disable case sensitivity for matching
    IGNORECASE = 1
}


# Match an include statement
# - the first argument ($1) should be the whole word include
# - the second argument ($2) can be everything, as long as delimited by quotes
$1 ~ /^(#:?)?include$/ &&
$2 ~ /^["'].+["']$/ {
    # Remove quotes from the included file name
    gsub(/'|"/, "", $2)
    # count included files per file to avoid having duplicates in our list
    if (incc[FILENAME,$2]++ == 0) {
        dirname = FILENAME
        gsub("/*[^/]*/*$", "", dirname)
        if (dirname == ""){
            dirname = "."
        }
        incpath = sprintf("%s/%s", dirname, $2)
        # Add the included file if the file exists
        if (getline < incpath >= 0) {
            inc[++ii] = sprintf("%s.inc += %s", FILENAME, incpath)
        }
    }
}

# Finally, produce the output for make, loop over all modules, use statements
# and include statements, empty lists are ignored in awk
END {
    for (i in inc) print inc[i]
}

