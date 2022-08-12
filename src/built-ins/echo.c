/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduboulo <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 23:20:31 by lduboulo          #+#    #+#             */
/*   Updated: 2022/08/12 19:05:32 by lduboulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../includes/minishell.h"

static bool	echo_check_arg(t_main *main, t_cell *cur)
{
	bool	check;
	char	*arg;

	check = FALSE;
	arg = ft_strnstr(main->input, cur->data, 10000);
	arg += ft_strlen(cur->data) + 1;
	if (arg[0] == ' ')
		check = TRUE;
	return (check);
}

static void	print_echo(t_main *main, t_cell *cur, int n, int i)
{
	int	fd;

	fd = STDOUT;
	if (main->proc.ncmd == 1 && main->fd.outfile > 1)
		fd = main->fd.outfile;
	if (cur->token == 39 || cur->token == 34)
	{
		ft_putstr_fd(cur->data, fd);
		if (echo_check_arg(main, cur) == TRUE && i < n)
			ft_putchar_fd(' ', fd);
	}
	else if (cur->next && (cur->next->token == 39 || cur->next->token == 34))
	{
		ft_putstr_fd(cur->data, fd);
		if (echo_check_arg(main, cur) == TRUE && i < n)
			ft_putchar_fd(' ', fd);
	}
	else if (i < n)
	{
		ft_putstr_fd(cur->data, fd);
		ft_putchar_fd(' ', fd);
	}
	else
		ft_putstr_fd(cur->data, fd);
}

static void	print_loop(t_main *main, t_cell *cur, int n, int icmd)
{
	int	i;

	i = 1;
	while (cur != NULL && cur->pos == icmd)
	{
		if (cur->token >= 994 && cur->token <= 997)
		{
			cur = cur->next->next;
			continue ;
		}
		else
			print_echo(main, cur, n, i++);
		cur = cur->next;
	}
}

static int	echo_n_args(t_cell *cur, int icmd)
{
	int	n;

	n = 0;
	while (cur != NULL && cur->pos == icmd)
	{
		if (cur->token >= 994 && cur->token <= 997)
		{
			cur = cur->next->next;
			continue ;
		}
		else
			n++;
		cur = cur->next;
	}
	return (n);
}

int	b_echo(t_main *main, t_cell *cur, int icmd)
{
	bool	arg;
	int		i;
	int		n;

	i = 2;
	cur = avoid_redir(cur->next, icmd);
	if (echo_protection(cur) == 0)
		return (0);
	arg = FALSE;
	if (ft_strncmp(cur->data, "-n", 2) == 0)
	{
		while (cur->data[i] && cur->data[i] == 'n')
			i++;
		if (cur->data[i] == '\0')
		{
			arg = TRUE;
			cur = cur->next;
		}
	}
	n = echo_n_args(cur, icmd);
	print_loop(main, cur, n, icmd);
	if (arg == FALSE)
		echo_end_print(main);
	g_exit_status = 0;
	return (0);
}
