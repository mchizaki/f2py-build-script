#!/usr/bin/awk -f

BEGIN {
    # Fortran is case insensitive, disable case sensitivity for matching
    IGNORECASE = 1
}

# Match a module statement
# - the first argument ($1) should be the whole word module
# - the second argument ($2) should be a valid module name
$1 ~ /^module$/ &&
$2 ~ /^[a-zA-Z][a-zA-Z0-9_]*$/ {
    # count module names per file to avoid having modules twice in our list
    if (modc[FILENAME,$2]++ == 0) {
        # add to the module list, the generated module name is expected
        # to be lowercase, the FILENAME is the current source file
        if ( $2 != "procedure") {
            mod[++im] = sprintf("%s.mod = $(%s)", tolower($2), FILENAME)
            # src[++im] = sprintf("mods += %s.mod", tolower($2))
        }
    }
}

# Match a program statement
# - the first argument ($1) should be the whole word program
# - the second argument ($2) should be a valid program name
$1 ~ /^program$/ &&
$2 ~ /^[a-zA-Z][a-zA-Z0-9_]*$/ {
    if (modc[FILENAME,$2]++ == 0) {
        prog[++im] = sprintf("%s.prog = $(%s)", tolower($2), FILENAME)
        prog[++im] = sprintf("$(%s).is_prog := %s", FILENAME, "True")
    }
}

# Match a use statement
# - the first argument ($1) should be the whole word use
# - the second argument ($2) should be a valid module name
$1 ~ /^use$/ &&
$2 ~ /^[a-zA-Z][a-zA-Z0-9_]*,?$/ {
    # Remove a trailing comma from an optional only statement
    gsub(/,/, "", $2)
    # count used module names per file to avoid using modules twice in our list
    if (usec[FILENAME,$2]++ == 0) {
        # add to the used modules, the generated module name is expected
        # to be lowercase, the FILENAME is the current source file
        if ( $2 != "procedure") {
            use[++iu] = sprintf("$(%s) += $(%s.mod)", FILENAME, tolower($2))
        }
    }
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
            inc[++ii] = sprintf("$(%s) += %s", FILENAME, incpath)
        }

        # # Add the included file to our list, this might be case-sensitive
        # inc[++ii] = sprintf("$(%s) += %s", FILENAME, $2)
    }
}

# Finally, produce the output for make, loop over all modules, use statements
# and include statements, empty lists are ignored in awk
END {
    for (i in mod ) print mod[i]
    for (i in prog) print prog[i]
    for (i in use ) print use[i]
    for (i in inc ) print inc[i]
}

