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

# Finally, produce the output for make, loop over all modules, use statements
# and include statements, empty lists are ignored in awk
END {
    for (i in mod ) print mod[i]
    for (i in prog) print prog[i]
    for (i in use ) print use[i]
}

