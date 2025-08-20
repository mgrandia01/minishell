[33mcommit c1c77bbb18e09cc5557fe26508ca7bab86dd55e4[m[33m ([m[1;36mHEAD -> [m[1;32mmarta[m[33m, [m[1;31morigin/marta[m[33m)[m
Author: Marta Grandia Gutierrez <mgrandia@cbr6s3.42barcelona.com>
Date:   Wed Aug 20 13:00:03 2025 +0200

    correccio mgrandia whith double '

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 94fc73a..7d35b9b 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -6,7 +6,7 @@[m
 /*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */[m
[31m-/*   Updated: 2025/08/13 16:07:06 by mgrandia         ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2025/08/20 11:55:13 by mgrandia         ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
[36m@@ -32,7 +32,7 @@[m [mvoid	join_tokens_with_end(t_token **tokens)[m
 			if (next->end == 1)[m
 				curr->end = 1;[m
 			curr->next = next->next;[m
[31m-			free(next->value);[m
[32m+[m			[32mfree(next->value);//double free[m
 			free(next);[m
 			next = curr->next;[m
 		}[m

[33mcommit 42de187900a4020acc26525f786074141ea65f70[m
Author: mgrandia01 <marta.grandia01@gmail.com>
Date:   Tue Aug 19 09:52:15 2025 +0200

    0 en argv[0] sin mensaje de error

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex ac34916..94fc73a 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -66,10 +66,7 @@[m [mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_dat *d)[m
 			}[m
 			if (t_val[d->i + 1] == '?')[m
 			{[m
[31m-				if (*n_lst && ft_strncmp((*n_lst)->value, "echo", 5) == 0)[m
[31m-					handle_echo_exit_status(d);[m
[31m-				else[m
[31m-					handle_exit_status(&(d->i));[m
[32m+[m				[32mhandle_echo_exit_status(d);[m
 			}[m
 			else[m
 			{[m

[33mcommit eff3cd8e17fdf198ce5ddee5f8a8b5c41475b3a2[m
Author: Marta Grandia Gutierrez <mgrandia@cbr13s3.42barcelona.com>
Date:   Wed Aug 13 16:16:00 2025 +0200

    wip minishell.h, falta la ultima funcio line too long

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 8329f0d..ac34916 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -6,7 +6,7 @@[m
 /*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */[m
[31m-/*   Updated: 2025/08/13 16:00:22 by mgrandia         ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2025/08/13 16:07:06 by mgrandia         ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
[36m@@ -41,7 +41,7 @@[m [mvoid	join_tokens_with_end(t_token **tokens)[m
 }[m
 [m
 // Initialize expansion state (quote type, index, result buffer, etc.)[m
[31m-void	init_exp_data(t_exp_data *data, char first_char, char *env[])[m
[32m+[m[32mvoid	init_exp_data(t_dat *data, char first_char, char *env[])[m
 {[m
 	data->quote = get_quoted_type(first_char);[m
 	data->i = 0;[m
[36m@@ -51,7 +51,7 @@[m [mvoid	init_exp_data(t_exp_data *data, char first_char, char *env[])[m
 }[m
 [m
 // Loop through token string and process variable expansions or literals[m
[31m-void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *d)[m
[32m+[m[32mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_dat *d)[m
 {[m
 	char	*literal;[m
 [m
[36m@@ -84,7 +84,7 @@[m [mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *d)[m
 }[m
 [m
 // Add final literal token if any characters were left unprocessed[m
[31m-void	finalize_expansion(t_token **n_lst, t_token *c, t_exp_data *data)[m
[32m+[m[32mvoid	finalize_expansion(t_token **n_lst, t_token *c, t_dat *data)[m
 {[m
 	if (data->s > 0)[m
 		add_tok(n_lst, c->type, literal_tok(&(data->result), &(data->s)), 0);[m

[33mcommit 5a3ceb3310ab7cd9a1952da25804f2f06fc23807[m
Author: Marta Grandia Gutierrez <mgrandia@cbr13s3.42barcelona.com>
Date:   Wed Aug 13 16:02:17 2025 +0200

    wip minishell.h norminette

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex fcfe5a0..8329f0d 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -6,7 +6,7 @@[m
 /*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */[m
[31m-/*   Updated: 2025/08/13 11:56:41 by mgrandia         ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2025/08/13 16:00:22 by mgrandia         ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
[36m@@ -51,35 +51,35 @@[m [mvoid	init_exp_data(t_exp_data *data, char first_char, char *env[])[m
 }[m
 [m
 // Loop through token string and process variable expansions or literals[m
[31m-void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
[32m+[m[32mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *d)[m
 {[m
 	char	*literal;[m
 [m
[31m-	while (t_val[data->i] != '\0')[m
[32m+[m	[32mwhile (t_val[d->i] != '\0')[m
 	{[m
[31m-		if (t_val[data->i] == '$' && data->quote != 1)[m
[32m+[m		[32mif (t_val[d->i] == '$' && d->quote != 1)[m
 		{[m
[31m-			if (data->s > 0)[m
[32m+[m			[32mif (d->s > 0)[m
 			{[m
[31m-				literal = literal_tok(&(data->result), &(data->s));[m
[31m-				add_token(n_lst, c->type, literal, 1);[m
[32m+[m				[32mliteral = literal_tok(&(d->result), &(d->s));[m
[32m+[m				[32madd_tok(n_lst, c->type, literal, 1);[m
 			}[m
[31m-			if (t_val[data->i + 1] == '?')[m
[32m+[m			[32mif (t_val[d->i + 1] == '?')[m
 			{[m
 				if (*n_lst && ft_strncmp((*n_lst)->value, "echo", 5) == 0)[m
[31m-					handle_echo_exit_status(data);[m
[32m+[m					[32mhandle_echo_exit_status(d);[m
 				else[m
[31m-					handle_exit_status(&(data->i));[m
[32m+[m					[32mhandle_exit_status(&(d->i));[m
 			}[m
 			else[m
 			{[m
[31m-				data->result = exp_var_at_index(t_val, &(data->i), data->env);[m
[31m-				handle_exp_result(n_lst, c, &(data->result), data->quote);[m
[31m-				data->s = 0;[m
[32m+[m				[32md->result = exp_var_at_index(t_val, &(d->i), d->env);[m
[32m+[m				[32mhandle_exp_result(n_lst, c, &(d->result), d->quote);[m
[32m+[m				[32md->s = 0;[m
 			}[m
 		}[m
 		else[m
[31m-			handle_literal_char(t_val, &(data->i), &(data->result), &(data->s));[m
[32m+[m			[32mhandle_literal_ch(t_val, &(d->i), &(d->result), &(d->s));[m
 	}[m
 }[m
 [m
[36m@@ -87,5 +87,5 @@[m [mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
 void	finalize_expansion(t_token **n_lst, t_token *c, t_exp_data *data)[m
 {[m
 	if (data->s > 0)[m
[31m-		add_token(n_lst, c->type, literal_tok(&(data->result), &(data->s)), 0);[m
[32m+[m		[32madd_tok(n_lst, c->type, literal_tok(&(data->result), &(data->s)), 0);[m
 }[m

[33mcommit 140ec321472016c45b866cef5f38f0c385fcd767[m
Author: Marta Grandia Gutierrez <mgrandia@cbr13s3.42barcelona.com>
Date:   Wed Aug 13 12:40:30 2025 +0200

     1 and problems with error fd

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 85010c8..fcfe5a0 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -6,7 +6,7 @@[m
 /*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */[m
[31m-/*   Updated: 2025/08/02 15:41:08 by mgrandia         ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2025/08/13 11:56:41 by mgrandia         ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
[36m@@ -65,7 +65,12 @@[m [mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
 				add_token(n_lst, c->type, literal, 1);[m
 			}[m
 			if (t_val[data->i + 1] == '?')[m
[31m-				handle_exit_status(&(data->i));[m
[32m+[m			[32m{[m
[32m+[m				[32mif (*n_lst && ft_strncmp((*n_lst)->value, "echo", 5) == 0)[m
[32m+[m					[32mhandle_echo_exit_status(data);[m
[32m+[m				[32melse[m
[32m+[m					[32mhandle_exit_status(&(data->i));[m
[32m+[m			[32m}[m
 			else[m
 			{[m
 				data->result = exp_var_at_index(t_val, &(data->i), data->env);[m

[33mcommit 5eb502695cffec184e999abf87fbc19d8e068ca2[m
Author: mgrandia01 <marta.grandia01@gmail.com>
Date:   Tue Aug 5 13:50:58 2025 +0200

    wip: fd

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 1c88dc2..85010c8 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -11,42 +11,6 @@[m
 /* ************************************************************************** */[m
 [m
 #include "../includes/minishell.h"[m
[31m-// pseudocódigo simplificado[m
[31m-[m
[31m-/*void join_tokens_with_end(t_token **tokens)[m
[31m-{[m
[31m-    t_token *curr;[m
[31m-    t_token *next;[m
[31m-    char    *joined;[m
[31m-    [m
[31m-    printf("DEBUG: Entrando a join_tokens_with_end\n");[m
[31m-    [m
[31m-    curr = *tokens;[m
[31m-    while (curr)[m
[31m-    {[m
[31m-        printf("DEBUG: Procesando token '%s' con end=%d\n", curr->value, curr->end);[m
[31m-        [m
[31m-        // Mientras el token actual tenga end == 1 Y haya un siguiente token[m
[31m-        while (curr->end == 1 && curr->next && curr->next->type != TOKEN_EOF)[m
[31m-        {[m
[31m-            next = curr->next;[m
[31m-            printf("DEBUG: Uniendo '%s' + '%s'\n", curr->value, next->value);[m
[31m-            [m
[31m-            joined = ft_strjoin(curr->value, next->value);[m
[31m-            free(curr->value);[m
[31m-            curr->value = joined;[m
[31m-            curr->end = next->end;[m
[31m-            curr->next = next->next;[m
[31m-            free(next->value);[m
[31m-            free(next);[m
[31m-            [m
[31m-            printf("DEBUG: Resultado: '%s' con end=%d\n", curr->value, curr->end);[m
[31m-        }[m
[31m-        curr = curr->next;[m
[31m-    }[m
[31m-    [m
[31m-    printf("DEBUG: Saliendo de join_tokens_with_end\n");[m
[31m-}*/[m
 [m
 void	join_tokens_with_end(t_token **tokens)[m
 {[m
[36m@@ -67,7 +31,6 @@[m [mvoid	join_tokens_with_end(t_token **tokens)[m
 			curr->value = joined;[m
 			if (next->end == 1)[m
 				curr->end = 1;[m
[31m-			//curr->end = next->end;[m
 			curr->next = next->next;[m
 			free(next->value);[m
 			free(next);[m

[33mcommit b16152ccd05e938bb2470b6896af2eef83f57852[m
Author: mgrandia01 <marta.grandia01@gmail.com>
Date:   Tue Aug 5 13:04:14 2025 +0200

    solved 'marta'hola

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 7b7878d..1c88dc2 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -11,8 +11,43 @@[m
 /* ************************************************************************** */[m
 [m
 #include "../includes/minishell.h"[m
[32m+[m[32m// pseudocódigo simplificado[m
[32m+[m
[32m+[m[32m/*void join_tokens_with_end(t_token **tokens)[m
[32m+[m[32m{[m
[32m+[m[32m    t_token *curr;[m
[32m+[m[32m    t_token *next;[m
[32m+[m[32m    char    *joined;[m
[32m+[m[41m    [m
[32m+[m[32m    printf("DEBUG: Entrando a join_tokens_with_end\n");[m
[32m+[m[41m    [m
[32m+[m[32m    curr = *tokens;[m
[32m+[m[32m    while (curr)[m
[32m+[m[32m    {[m
[32m+[m[32m        printf("DEBUG: Procesando token '%s' con end=%d\n", curr->value, curr->end);[m
[32m+[m[41m        [m
[32m+[m[32m        // Mientras el token actual tenga end == 1 Y haya un siguiente token[m
[32m+[m[32m        while (curr->end == 1 && curr->next && curr->next->type != TOKEN_EOF)[m
[32m+[m[32m        {[m
[32m+[m[32m            next = curr->next;[m
[32m+[m[32m            printf("DEBUG: Uniendo '%s' + '%s'\n", curr->value, next->value);[m
[32m+[m[41m            [m
[32m+[m[32m            joined = ft_strjoin(curr->value, next->value);[m
[32m+[m[32m            free(curr->value);[m
[32m+[m[32m            curr->value = joined;[m
[32m+[m[32m            curr->end = next->end;[m
[32m+[m[32m            curr->next = next->next;[m
[32m+[m[32m            free(next->value);[m
[32m+[m[32m            free(next);[m
[32m+[m[41m            [m
[32m+[m[32m            printf("DEBUG: Resultado: '%s' con end=%d\n", curr->value, curr->end);[m
[32m+[m[32m        }[m
[32m+[m[32m        curr = curr->next;[m
[32m+[m[32m    }[m
[32m+[m[41m    [m
[32m+[m[32m    printf("DEBUG: Saliendo de join_tokens_with_end\n");[m
[32m+[m[32m}*/[m
 [m
[31m-// FIXME Une tokens consecutivos con end == 1[m
 void	join_tokens_with_end(t_token **tokens)[m
 {[m
 	t_token	*curr;[m
[36m@@ -30,7 +65,9 @@[m [mvoid	join_tokens_with_end(t_token **tokens)[m
 			joined = ft_strjoin(curr->value, next->value);[m
 			free(curr->value);[m
 			curr->value = joined;[m
[31m-			curr->end = next->end;[m
[32m+[m			[32mif (next->end == 1)[m
[32m+[m				[32mcurr->end = 1;[m
[32m+[m			[32m//curr->end = next->end;[m
 			curr->next = next->next;[m
 			free(next->value);[m
 			free(next);[m

[33mcommit 644753e4416ae25160f2385b08a630dbace936e9[m
Author: Marta Grandia Gutierrez <mgrandia@cbr1s6.42barcelona.com>
Date:   Sat Aug 2 16:17:39 2025 +0200

    v1-split_utils

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 6dd3493..7b7878d 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -6,13 +6,13 @@[m
 /*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */[m
 /*                                                +#+#+#+#+#+   +#+           */[m
 /*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */[m
[31m-/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */[m
[32m+[m[32m/*   Updated: 2025/08/02 15:41:08 by mgrandia         ###   ########.fr       */[m
 /*                                                                            */[m
 /* ************************************************************************** */[m
 [m
 #include "../includes/minishell.h"[m
 [m
[31m-// Une tokens consecutivos con end == 1[m
[32m+[m[32m// FIXME Une tokens consecutivos con end == 1[m
 void	join_tokens_with_end(t_token **tokens)[m
 {[m
 	t_token	*curr;[m

[33mcommit 92ce0f1d14955c3c8184d8f9d9451bfa903ca7a6[m
Author: mgrandia01 <marta.grandia01@gmail.com>
Date:   Thu Jul 31 09:30:23 2025 +0200

    norminette

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 4309c5b..6dd3493 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -11,31 +11,33 @@[m
 /* ************************************************************************** */[m
 [m
 #include "../includes/minishell.h"[m
[32m+[m
 // Une tokens consecutivos con end == 1[m
[31m-void    join_tokens_with_end(t_token **tokens)[m
[32m+[m[32mvoid	join_tokens_with_end(t_token **tokens)[m
 {[m
[31m-        t_token *curr;[m
[31m-        t_token *next;[m
[31m-        char    *joined;[m
[31m-        curr = *tokens;[m
[31m-        while (curr && curr->next)[m
[31m-        {[m
[31m-                next = curr->next;[m
[31m-                while (next && curr->end == 1)[m
[31m-                {[m
[31m-                        if (next->type == TOKEN_EOF)[m
[31m-                                break ;[m
[31m-                        joined = ft_strjoin(curr->value, next->value);[m
[31m-                        free(curr->value);[m
[31m-                        curr->value = joined;[m
[31m-                        curr->end = next->end; // el "end final" será el del último token[m
[31m-                        curr->next = next->next;[m
[31m-                        free(next->value);[m
[31m-                        free(next);[m
[31m-                        next = curr->next;[m
[31m-                }[m
[31m-                curr = curr->next;[m
[31m-        }[m
[32m+[m	[32mt_token	*curr;[m
[32m+[m	[32mt_token	*next;[m
[32m+[m	[32mchar	*joined;[m
[32m+[m
[32m+[m	[32mcurr = *tokens;[m
[32m+[m	[32mwhile (curr && curr->next)[m
[32m+[m	[32m{[m
[32m+[m		[32mnext = curr->next;[m
[32m+[m		[32mwhile (next && curr->end == 1)[m
[32m+[m		[32m{[m
[32m+[m			[32mif (next->type == TOKEN_EOF)[m
[32m+[m				[32mbreak ;[m
[32m+[m			[32mjoined = ft_strjoin(curr->value, next->value);[m
[32m+[m			[32mfree(curr->value);[m
[32m+[m			[32mcurr->value = joined;[m
[32m+[m			[32mcurr->end = next->end;[m
[32m+[m			[32mcurr->next = next->next;[m
[32m+[m			[32mfree(next->value);[m
[32m+[m			[32mfree(next);[m
[32m+[m			[32mnext = curr->next;[m
[32m+[m		[32m}[m
[32m+[m		[32mcurr = curr->next;[m
[32m+[m	[32m}[m
 }[m
 [m
 // Initialize expansion state (quote type, index, result buffer, etc.)[m
[36m@@ -49,31 +51,31 @@[m [mvoid	init_exp_data(t_exp_data *data, char first_char, char *env[])[m
 }[m
 [m
 // Loop through token string and process variable expansions or literals[m
[31m-void    p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
[32m+[m[32mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
 {[m
[31m-        char    *literal;[m
[32m+[m	[32mchar	*literal;[m
 [m
[31m-        while (t_val[data->i] != '\0')[m
[31m-        {[m
[31m-                if (t_val[data->i] == '$' && data->quote != 1)[m
[31m-                {[m
[31m-                        if (data->s > 0)[m
[31m-                        {[m
[31m-                                literal = literal_tok(&(data->result), &(data->s));[m
[31m-                                add_token(n_lst, c->type, literal, 1);[m
[31m-                        }[m
[31m-                        if (t_val[data->i + 1] == '?')[m
[31m-                                handle_exit_status(&(data->i));[m
[31m-                        else[m
[31m-                        {[m
[31m-                                data->result = exp_var_at_index(t_val, &(data->i), data->env);[m
[31m-                                handle_exp_result(n_lst, c, &(data->result), data->quote);[m
[31m-                                data->s = 0;[m
[31m-                        }[m
[31m-                }[m
[31m-                else[m
[31m-                        handle_literal_char(t_val, &(data->i), &(data->result), &(data->s));[m
[31m-        }[m
[32m+[m	[32mwhile (t_val[data->i] != '\0')[m
[32m+[m	[32m{[m
[32m+[m		[32mif (t_val[data->i] == '$' && data->quote != 1)[m
[32m+[m		[32m{[m
[32m+[m			[32mif (data->s > 0)[m
[32m+[m			[32m{[m
[32m+[m				[32mliteral = literal_tok(&(data->result), &(data->s));[m
[32m+[m				[32madd_token(n_lst, c->type, literal, 1);[m
[32m+[m			[32m}[m
[32m+[m			[32mif (t_val[data->i + 1] == '?')[m
[32m+[m				[32mhandle_exit_status(&(data->i));[m
[32m+[m			[32melse[m
[32m+[m			[32m{[m
[32m+[m				[32mdata->result = exp_var_at_index(t_val, &(data->i), data->env);[m
[32m+[m				[32mhandle_exp_result(n_lst, c, &(data->result), data->quote);[m
[32m+[m				[32mdata->s = 0;[m
[32m+[m			[32m}[m
[32m+[m		[32m}[m
[32m+[m		[32melse[m
[32m+[m			[32mhandle_literal_char(t_val, &(data->i), &(data->result), &(data->s));[m
[32m+[m	[32m}[m
 }[m
 [m
 // Add final literal token if any characters were left unprocessed[m

[33mcommit 238677a825a62f900c135924b151ad444653eef0[m
Author: mgrandia01 <marta.grandia01@gmail.com>
Date:   Wed Jul 30 17:12:06 2025 +0200

    wip: marta

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 0eda960..4309c5b 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -12,32 +12,30 @@[m
 [m
 #include "../includes/minishell.h"[m
 // Une tokens consecutivos con end == 1[m
[31m-[m
[31m-void	join_tokens_with_end(t_token **tokens)[m
[32m+[m[32mvoid    join_tokens_with_end(t_token **tokens)[m
 {[m
[31m-	t_token	*curr;[m
[31m-	t_token	*next;[m
[31m-	char	*joined;[m
[31m-[m
[31m-	curr = *tokens;[m
[31m-	while (curr && curr->next)[m
[31m-	{[m
[31m-		next = curr->next;[m
[31m-		while (next && curr->end == 1)[m
[31m-		{[m
[31m-			joined = ft_strjoin(curr->value, next->value);[m
[31m-			free(curr->value);[m
[31m-			curr->value = joined;[m
[31m-[m
[31m-			curr->end = next->end; // el "end final" será el del último token[m
[31m-[m
[31m-			curr->next = next->next;[m
[31m-			free(next->value);[m
[31m-			free(next);[m
[31m-			next = curr->next;[m
[31m-		}[m
[31m-		curr = curr->next;[m
[31m-	}[m
[32m+[m[32m        t_token *curr;[m
[32m+[m[32m        t_token *next;[m
[32m+[m[32m        char    *joined;[m
[32m+[m[32m        curr = *tokens;[m
[32m+[m[32m        while (curr && curr->next)[m
[32m+[m[32m        {[m
[32m+[m[32m                next = curr->next;[m
[32m+[m[32m                while (next && curr->end == 1)[m
[32m+[m[32m                {[m
[32m+[m[32m                        if (next->type == TOKEN_EOF)[m
[32m+[m[32m                                break ;[m
[32m+[m[32m                        joined = ft_strjoin(curr->value, next->value);[m
[32m+[m[32m                        free(curr->value);[m
[32m+[m[32m                        curr->value = joined;[m
[32m+[m[32m                        curr->end = next->end; // el "end final" será el del último token[m
[32m+[m[32m                        curr->next = next->next;[m
[32m+[m[32m                        free(next->value);[m
[32m+[m[32m                        free(next);[m
[32m+[m[32m                        next = curr->next;[m
[32m+[m[32m                }[m
[32m+[m[32m                curr = curr->next;[m
[32m+[m[32m        }[m
 }[m
 [m
 // Initialize expansion state (quote type, index, result buffer, etc.)[m

[33mcommit fb653ceb47de17ab558c0a3e6e1e8b4ef49afcb2[m
Author: mgrandia01 <marta.grandia01@gmail.com>
Date:   Wed Jul 30 15:45:07 2025 +0200

    quotes

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mindex 68bc2f8..0eda960 100644[m
[1m--- a/srcs/expansion_exec.c[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -11,6 +11,34 @@[m
 /* ************************************************************************** */[m
 [m
 #include "../includes/minishell.h"[m
[32m+[m[32m// Une tokens consecutivos con end == 1[m
[32m+[m
[32m+[m[32mvoid	join_tokens_with_end(t_token **tokens)[m
[32m+[m[32m{[m
[32m+[m	[32mt_token	*curr;[m
[32m+[m	[32mt_token	*next;[m
[32m+[m	[32mchar	*joined;[m
[32m+[m
[32m+[m	[32mcurr = *tokens;[m
[32m+[m	[32mwhile (curr && curr->next)[m
[32m+[m	[32m{[m
[32m+[m		[32mnext = curr->next;[m
[32m+[m		[32mwhile (next && curr->end == 1)[m
[32m+[m		[32m{[m
[32m+[m			[32mjoined = ft_strjoin(curr->value, next->value);[m
[32m+[m			[32mfree(curr->value);[m
[32m+[m			[32mcurr->value = joined;[m
[32m+[m
[32m+[m			[32mcurr->end = next->end; // el "end final" será el del último token[m
[32m+[m
[32m+[m			[32mcurr->next = next->next;[m
[32m+[m			[32mfree(next->value);[m
[32m+[m			[32mfree(next);[m
[32m+[m			[32mnext = curr->next;[m
[32m+[m		[32m}[m
[32m+[m		[32mcurr = curr->next;[m
[32m+[m	[32m}[m
[32m+[m[32m}[m
 [m
 // Initialize expansion state (quote type, index, result buffer, etc.)[m
 void	init_exp_data(t_exp_data *data, char first_char, char *env[])[m
[36m@@ -23,31 +51,31 @@[m [mvoid	init_exp_data(t_exp_data *data, char first_char, char *env[])[m
 }[m
 [m
 // Loop through token string and process variable expansions or literals[m
[31m-void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
[32m+[m[32mvoid    p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
 {[m
[31m-	char	*literal;[m
[32m+[m[32m        char    *literal;[m
 [m
[31m-	while (t_val[data->i] != '\0')[m
[31m-	{[m
[31m-		if (t_val[data->i] == '$' && data->quote != 1)[m
[31m-		{[m
[31m-			if (data->s > 0)[m
[31m-			{[m
[31m-				literal = literal_tok(&(data->result), &(data->s));[m
[31m-				add_token(n_lst, c->type, literal, 1);[m
[31m-			}[m
[31m-			if (t_val[data->i + 1] == '?')[m
[31m-				handle_exit_status(&(data->i));[m
[31m-			else[m
[31m-			{[m
[31m-				data->result = exp_var_at_index(t_val, &(data->i), data->env);[m
[31m-				handle_exp_result(n_lst, c, &(data->result), data->quote);[m
[31m-				data->s = 0;[m
[31m-			}[m
[31m-		}[m
[31m-		else[m
[31m-			handle_literal_char(t_val, &(data->i), &(data->result), &(data->s));[m
[31m-	}[m
[32m+[m[32m        while (t_val[data->i] != '\0')[m
[32m+[m[32m        {[m
[32m+[m[32m                if (t_val[data->i] == '$' && data->quote != 1)[m
[32m+[m[32m                {[m
[32m+[m[32m                        if (data->s > 0)[m
[32m+[m[32m                        {[m
[32m+[m[32m                                literal = literal_tok(&(data->result), &(data->s));[m
[32m+[m[32m                                add_token(n_lst, c->type, literal, 1);[m
[32m+[m[32m                        }[m
[32m+[m[32m                        if (t_val[data->i + 1] == '?')[m
[32m+[m[32m                                handle_exit_status(&(data->i));[m
[32m+[m[32m                        else[m
[32m+[m[32m                        {[m
[32m+[m[32m                                data->result = exp_var_at_index(t_val, &(data->i), data->env);[m
[32m+[m[32m                                handle_exp_result(n_lst, c, &(data->result), data->quote);[m
[32m+[m[32m                                data->s = 0;[m
[32m+[m[32m                        }[m
[32m+[m[32m                }[m
[32m+[m[32m                else[m
[32m+[m[32m                        handle_literal_char(t_val, &(data->i), &(data->result), &(data->s));[m
[32m+[m[32m        }[m
 }[m
 [m
 // Add final literal token if any characters were left unprocessed[m

[33mcommit f2c0473dcf87fa1bf54099708f98ebcab5ae183e[m
Author: mgrandia01 <marta.grandia01@gmail.com>
Date:   Tue Jul 29 17:12:30 2025 +0200

    norminette

[1mdiff --git a/srcs/expansion_exec.c b/srcs/expansion_exec.c[m
[1mnew file mode 100644[m
[1mindex 0000000..68bc2f8[m
[1m--- /dev/null[m
[1m+++ b/srcs/expansion_exec.c[m
[36m@@ -0,0 +1,58 @@[m
[32m+[m[32m/* ************************************************************************** */[m
[32m+[m[32m/*                                                                            */[m
[32m+[m[32m/*                                                        :::      ::::::::   */[m
[32m+[m[32m/*   expansion_exec.c                                   :+:      :+:    :+:   */[m
[32m+[m[32m/*                                                    +:+ +:+         +:+     */[m
[32m+[m[32m/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */[m
[32m+[m[32m/*                                                +#+#+#+#+#+   +#+           */[m
[32m+[m[32m/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */[m
[32m+[m[32m/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */[m
[32m+[m[32m/*                                                                            */[m
[32m+[m[32m/* ************************************************************************** */[m
[32m+[m
[32m+[m[32m#include "../includes/minishell.h"[m
[32m+[m
[32m+[m[32m// Initialize expansion state (quote type, index, result buffer, etc.)[m
[32m+[m[32mvoid	init_exp_data(t_exp_data *data, char first_char, char *env[])[m
[32m+[m[32m{[m
[32m+[m	[32mdata->quote = get_quoted_type(first_char);[m
[32m+[m	[32mdata->i = 0;[m
[32m+[m	[32mdata->result = NULL;[m
[32m+[m	[32mdata->s = 0;[m
[32m+[m	[32mdata->env = env;[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m// Loop through token string and process variable expansions or literals[m
[32m+[m[32mvoid	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)[m
[32m+[m[32m{[m
[32m+[m	[32mchar	*literal;[m
[32m+[m
[32m+[m	[32mwhile (t_val[data->i] != '\0')[m
[32m+[m	[32m{[m
[32m+[m		[32mif (t_val[data->i] == '$' && data->quote != 1)[m
[32m+[m		[32m{[m
[32m+[m			[32mif (data->s > 0)[m
[32m+[m			[32m{[m
[32m+[m				[32mliteral = literal_tok(&(data->result), &(data->s));[m
[32m+[m				[32madd_token(n_lst, c->type, literal, 1);[m
[32m+[m			[32m}[m
[32m+[m			[32mif (t_val[data->i + 1] == '?')[m
[32m+[m				[32mhandle_exit_status(&(data->i));[m
[32m+[m			[32melse[m
[32m+[m			[32m{[m
[32m+[m				[32mdata->result = exp_var_at_index(t_val, &(data->i), data->env);[m
[32m+[m				[32mhandle_exp_result(n_lst, c, &(data->result), data->quote);[m
[32m+[m				[32mdata->s = 0;[m
[32m+[m			[32m}[m
[32m+[m		[32m}[m
[32m+[m		[32melse[m
[32m+[m			[32mhandle_literal_char(t_val, &(data->i), &(data->result), &(data->s));[m
[32m+[m	[32m}[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m// Add final literal token if any characters were left unprocessed[m
[32m+[m[32mvoid	finalize_expansion(t_token **n_lst, t_token *c, t_exp_data *data)[m
[32m+[m[32m{[m
[32m+[m	[32mif (data->s > 0)[m
[32m+[m		[32madd_token(n_lst, c->type, literal_tok(&(data->result), &(data->s)), 0);[m
[32m+[m[32m}[m
