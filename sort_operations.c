/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 12:34:28 by vfleskes          #+#    #+#             */
/*   Updated: 2026/11/27 13:31:37 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	swap_nodes(t_node **stack)
{
	t_node	*first;
	t_node	*second;

	if (!stack || !*stack || !(*stack)->next)
		return (0);
	first = *stack;
	second = first->next;
	first->next = second->next;
	second->next = first;
	*stack = second;
	return (1);
}

void	sa(t_node **a)
{
	if (swap_nodes(a))
	{
		write(1, "sa\n", 3);
		bench_count(OP_SA);
	}
}

void	sb(t_node **b)
{
	if (swap_nodes(b))
	{
		write(1, "sb\n", 3);
		bench_count(OP_SB);
	}
}

void	ss(t_node **a, t_node **b)
{
	int	swapped_a;
	int	swapped_b;

	swapped_a = swap_nodes(a);
	swapped_b = swap_nodes(b);
	if (swapped_a || swapped_b)
	{
		write(1, "ss\n", 3);
		bench_count(OP_SS);
	}
}

/*Met le premier numero de b dans a*/
void	pa(t_node **a, t_node **b)
{
	t_node	*node;

	if (!a || !b || !*b)
		return ;
	node = *b;
	*b = (*b)->next;
	node->next = *a;
	*a = node;
	write(1, "pa\n", 3);
	bench_count(OP_PA);
}
