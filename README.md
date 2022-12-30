# OS_Linux_Shell

In this project, a simple Unix shell is designed that can execute the entered commands and display the output to the user like a normal shell. In addition to normal commands such as (ls, cd, pwd, ..), this shell performs the following commands to make text processing easier.
to execute a command, a seprate process is created using "fork" and commands will execute on it.

## Get the first word of each line
This command shows the first word of each line of the input file. 
You can run this command as follows
```
gfwol fileAddress 
```

## Common string in a file
This command shows the most common string in the input file.
You can run this command as follows
```
cs fileAddress
```

## Delete empty spaces
This command deletes all empty spaces from the file and shows it.
You can run this command as follows
```
des fileAddress
```

## Show not comment lines
This command shows all the lines of the input file that are not commented.
You can run this command as follows
```
snc fileAddress
```

## Number of lines
This command shows the number of lines of the input file.
You can run this command as follows
```
nor fileAddress
```

## Show top 10 line
This command shows the top 10 lines of the input file.
You can run this command as follows
```
s10l fileAddress
```
## some option of this shell
This shell doesn't not close by pressing ctrl+c and closes by ``` exit ``` command. When receiving this combination, the current line is interrupted and a new prompt is printed.

This shell also supports the history feature and saves them in a file with the specified address, and the history is not deleted when the shell is closed.
