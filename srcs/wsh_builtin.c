#include "minishell.h"

int		ft_isbuiltin(const char *command)
{
	static char builin[7][10] = {"echo", "cd", "pwd", "exit", "export", "unset", "env"};
	int			i;

	i = 0;
	while (ft_strncmp(command, builin[i], ft_strlen(builin[i])) != 0)
		i++;
	if (i == 7)
		return (0);
	return (1);
}

int		wsh_exec_builtin(t_wsh_list *wsh_list)
{
	if (!ft_strncmp(wsh_list->ast_parsed->wsh_command, "pwd", 4))
		wsh_pwd();
	if (!ft_strncmp(wsh_list->ast_parsed->wsh_command, "env", 4))
		wsh_env(wsh_list);
	if (!ft_strncmp(wsh_list->ast_parsed->wsh_command, "echo", 5))
		wsh_echo(wsh_list->ast_parsed);
	return (0);
}