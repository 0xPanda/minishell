/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wsh_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mashad <mashad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 16:25:23 by mashad            #+#    #+#             */
/*   Updated: 2021/01/08 14:51:51 by mashad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		wsh_tokenizer(char cmd[][50], char *string, int pipe)
{
    char    *token;
	int		i;

	i = 0;
	if (!(token = (char *)malloc(sizeof(char) * 50)))
		return (ERROR);
    while (wsh_scan_commands(token, (const char *) string, pipe))
    	ft_strlcpy((char *)cmd[i++], (const char *)token, ft_strlen(token)+1);
	ft_strlcpy((char *)cmd[i++], (const char *)token, ft_strlen(token)+1);
	cmd[i][0]= 0;
	free(token);
	token = NULL;
    return (1);
}

int	g_i;
int g_j;

int				ft_ispipe(t_wsh_tokens *wsh_token, char token[][50], int ret)
{
	int pip[2];

	while (token[g_i][0] != '\0')
		g_i++;
	if (g_i == g_j)
		return (ret);
	pipe(pip);
	if (wsh_token->std_in == 0)
		wsh_token->next->std_in = pip[1];
	g_j++;
	return (pip[0]);
}

t_wsh_tokens	*wsh_fillCommands(t_wsh_tokens *wsh_token, char pipe[][50])
{
	t_wsh_tokens	*token;
	int		counter;
	int		i;
	int		j;

	counter = 0;
	g_i = 0;
	g_j = 1;
	token = wsh_token;
	while (pipe[counter][0] != '\0')
	{
		wsh_token->wsh_command = ft_split((char *)pipe[counter], ' ')[0];
		i = 1;
		j = 0;
		if (ft_split((char *)pipe[counter], ' ')[i])
		{
			while (ft_split((char *)pipe[counter], ' ')[i][0] == '-')
				wsh_token->wsh_arg[j++] = ft_split((char *)pipe[counter], ' ')[i++];
			j = 0;
			while (ft_split((char *)pipe[counter], ' ')[i])
				wsh_token->wsh_param[j++] = ft_split((char *)pipe[counter], ' ')[i++];
		}
		if (!(wsh_token->next = (t_wsh_tokens *)malloc(sizeof(t_wsh_tokens))))
			return (NULL);
		wsh_token->std_out = ft_ispipe(wsh_token, pipe, 1);
		wsh_token = wsh_token->next;
		counter++;
	}
	return (wsh_token);
}

t_wsh_tokens	*wsh_parse( char *cmd )
{
    char			array[50][50];
	char			pipe[50][50];
	t_wsh_tokens	*wsh_token;
	t_wsh_tokens	*wsh_token_first;
    int				i;
	int				j;
    
    i = 0;
	j = 0;
	if (!(wsh_token = (t_wsh_tokens *)malloc(sizeof(t_wsh_tokens))))
		return (NULL);
    if (wsh_tokenizer(array, cmd, 0) == ERROR)
		return (NULL);
	wsh_token_first = wsh_token;
	while (array[i][0] != '\0')
	{
		if (wsh_tokenizer(pipe, array[i], 1) == ERROR)
			return (NULL);
		wsh_token->std_in = 0;
		wsh_token = wsh_fillCommands(wsh_token, pipe);
		i++;
	}
	return (wsh_token_first);
}