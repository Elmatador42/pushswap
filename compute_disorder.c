/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_disorder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 18:01:20 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 13:30:57 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

double	compute_disorder(t_node *a)
{
	t_node	*i;
	t_node	*j;
	long	mistakes;
	long	total;

	mistakes = 0;
	total = 0;
	i = a;
	while (i)
	{
		j = i->next;
		while (j)
		{
			total++;
			if (i->value > j->value)
				mistakes++;
			j = j->next;
		}
		i = i->next;
	}
	if (total == 0)
		return (0.0);
	return ((double)mistakes / (double)total);
}
