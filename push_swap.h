/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arngomes <arngomes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 09:10:53 by arngomes          #+#    #+#             */
/*   Updated: 2026/11/27 13:30:16 by arngomes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <unistd.h>
# include <stddef.h>
# include <limits.h>
# include <stdlib.h>

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

typedef enum e_op
{
	OP_SA, OP_SB, OP_SS, OP_PA, OP_PB,
	OP_RA, OP_RB, OP_RR, OP_RRA, OP_RRB, OP_RRR, OP_COUNT
}	t_op;

void		bench_count(t_op op);
void		bench_print(double disorder, const char *strategy);

void		sa(t_node **a);
void		sb(t_node **b);
void		ss(t_node **a, t_node **b);
void		pa(t_node **a, t_node **b);
void		pb(t_node **a, t_node **b);
void		ra(t_node **a);
void		rb(t_node **b);
void		rr(t_node **a, t_node **b);
void		rra(t_node **a);
void		rrb(t_node **b);
void		rrr(t_node **a, t_node **b);

void		sort_simple(t_node **a, t_node **b);
void		sort_small(t_node **a, t_node **b);
void		sort_medium(t_node **a, t_node **b);
void		sort_complex(t_node **a, t_node **b);
const char	*sort_adaptive(t_node **a, t_node **b);
void		add_bench(t_node **a, t_node **b, int num);

double		compute_disorder(t_node *a);

int			stack_size(t_node *s);
void		assign_index(t_node *a);
int			ft_isqrt(int n);
int			find_index_pos(t_node *s, int index);
int			is_sorted(t_node *stack);
int			check_doubles(t_node **a);
int			chose_algo(int num, t_node **a, t_node **b);

void		ft_lstadd_back(t_node **lst, t_node *new);
t_node		*ft_lstnew(int num);
void		ft_lstclear(t_node **lst);

#endif