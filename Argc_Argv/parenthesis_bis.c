#include <unistd.h>
#include <stdio.h>

int	ft_len(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/*
** Version simplifiee et correcte : on suit le "solde" de parentheses.
** On incremente sur '(', decremente sur ')'.
** Si le solde devient negatif, une ')' est arrivee avant sa '(' -> invalide.
** A la fin, le solde doit etre revenu a 0.
** Les autres caracteres (espaces, etc.) sont simplement ignores.
*/
int	valid_parenthesis(char *s)
{
	int	i;
	int	balance;

	i = 0;
	balance = 0;
	while (s[i])
	{
		if (s[i] == '(')
			balance++;
		else if (s[i] == ')')
		{
			balance--;
			if (balance < 0)
				return (0);
		}
		i++;
	}
	return (balance == 0);
}

/*
** Genere tous les sous-ensembles possibles en remplacant ou non
** chaque caractere par un espace.
** Point important : on sauvegarde tab[index] avant de le remplacer
** par ' ', puis on le restaure apres l'appel recursif. Sans ca,
** le tableau reste modifie pour les appels suivants au meme niveau
** de recursion, et certaines combinaisons ne sont jamais generees
** (ou sont generees en double).
*/
void	findSubsets(char tab[], int n, int index)
{
	char	saved;

	if (index == 0 && valid_parenthesis(tab))
	{
		puts(tab);
		return ;
	}
	if (index == n)
	{
		if (valid_parenthesis(tab))
			puts(tab);
		return ;
	}
	findSubsets(tab, n, index + 1);
	saved = tab[index];
	tab[index] = ' ';
	findSubsets(tab, n, index + 1);
	tab[index] = saved;
}

int	main(int argc, char **argv)
{
	int	n;
	char *s;

	if (argc != 2)
	{
		puts("wrong number of arguments");
		return (0);
	}
	s = argv[1];
	n = ft_len(s);
	findSubsets(s, n, 0);
	return (0);
}