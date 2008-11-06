function matrix = readCsv(filename)
    f_in = fopen(filename, "rt");

    if ( f_in == -1)
        printf("readCsv: Error, could not open file %s\n", filename);
        matrix=[];
        return;
    endif

    c = fscanf(f_in, "%c",1);

    # If c is a plus or minus, read in the next character, could be the start
    # of number or text.
    if(c == "-" || c == "+")
        c = fscanf(f_in, "%c",1);
    endif

    # Throw away the first line of the file if it is text.
    if(isalpha(c) || c == "\"" || c == "\'")
        while( c != "\n" && ! feof(f_in))
            c = fscanf(f_in, "%c", "C");
        endwhile

    # If we found numbers, great!
    elseif( isdigit(c) || c == "-" || "+")
        # ok, assume the rest of the file contains numbers, go back to the
        # begining of the file.
        fseek(f_in, 0);

    # Uh oh, an unhandled case!
    else
        printf("readCsv: Error, unexpected char '%c'\n", c);
        matrix = [];
        return;
    endif

    # Calculate the size of the matrix
    top_of_file = ftell(f_in);

    n = 0;

    trash = fscanf(f_in,"%f,");
    n = length(trash);

    m = 0;

    while(! feof(f_in))
        fscanf(f_in, "%s,");
        ++m;
    endwhile

    # Go back to the top of the file

    fseek(f_in,top_of_file);

    row = 1;

    matrix = zeros(m,n);
    while(row <= m)

        trash = fscanf(f_in, "%f,");
        trash = trash';

        matrix(row,:) = trash;

        row++;
    endwhile

    fclose(f_in);
endfunction
