#include "minishell.h"


char		*check_bin(char *cmd, char *command)
{
	DIR				*folder;
	struct dirent	*entry;
	char			*path;

	path = NULL;
	folder = opendir(cmd);
	if (!folder)
		return (NULL);
	while ((entry = readdir(folder)))
	{
		if (ft_strncmp(entry->d_name, command, ft_strlen(command)) == 0)
			path = cmd;
	}
	closedir(folder);
	return (path);
}

char        **wsh_set_arr(char *arr[], char *path, t_wsh_list *wsh_list)
{
    int i;
    int j;

    i = 0;
    j = 1;
	if (!(arr = malloc(sizeof(char **) * 1024)))
		return (NULL);
	if (!(arr[0] = malloc(sizeof(char *) * ft_strlen(path) + 1)))
		return (NULL);
    arr[0] = path;
    while (wsh_list->ast_parsed->wsh_arg && wsh_list->ast_parsed->wsh_arg[i])
	{
		if (!(arr[i] = malloc(sizeof(char *)  * ft_strlen(wsh_list->ast_parsed->wsh_arg[i] + 1))))
			return (NULL);
        arr[j++] = wsh_list->ast_parsed->wsh_arg[i++];
	}
	i = 0;
    while (wsh_list->ast_parsed->wsh_param && wsh_list->ast_parsed->wsh_param[i])
	{
		if (!(arr[i] = malloc(sizeof(char *)  * ft_strlen(wsh_list->ast_parsed->wsh_param[i] + 1))))
			return (NULL);
        arr[j++] = wsh_list->ast_parsed->wsh_param[i++];
	}
    arr[j] = NULL;
	return (arr);
}

void        wsh_execve(t_wsh_list *wsh_list)
{
    int     i;
    char    **cmd;
    char    *path;
    char    *tmp;
    char    **arr;

    i = 0;
	arr = NULL;
    while (wsh_list->wsh_envs && wsh_list->wsh_envs[i] && ft_strncmp(wsh_list->wsh_envs[i], "PATH=", 5) != 0)
		i++;
    cmd = ft_split(wsh_list->wsh_envs[i], ':');
    path = check_bin(cmd[0] + 5, wsh_list->ast_parsed->wsh_command);
    i = 50;
    while (wsh_list->ast_parsed->wsh_command && cmd[i] && path == NULL)
		path = check_bin(cmd[i++], wsh_list->ast_parsed->wsh_command);
    tmp = ft_strjoin(path, "/");
    path = ft_strjoin(tmp, wsh_list->ast_parsed->wsh_command);
    wsh_free((void*) tmp);
    arr = wsh_set_arr(arr, path, wsh_list);
	i = 0;
	while (arr[i])
	{
    	ft_putstr_fd(arr[i++], 1);
		ft_putchar_fd('\n', 1);
	}
    i = fork();
    if (i == 0)    
        if (execve(path , arr, wsh_list->wsh_envs) == -1)
        {
            ft_putstr_fd("error\n", 2);
            exit(1);
        }
    return ;
}