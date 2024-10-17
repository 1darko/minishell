#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
       #include <sys/stat.h>
       #include <fcntl.h>

long long ft_atol(char *str)
{
    long long ret;
    int i;
    int neg;

	i = 0;
	neg = 1;
	ret = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		ret = ret * 10 + (str[i] - 48);
		i++;
	}
	return (ret * neg);
}

int ret_exit(char *x)
{
    int i;

    i = 0;
    while(x[i] != '\0')
    {
        if(!(x[i] >= '0' && x[i] <= '9'))
        {
            printf("Error: Invalid exit argument. Numeric argument required.\n");
            return (2);
        }
        i++;
    }
    return (ft_atol(x));
}
void ft_exit(char *x)
{
    unsigned int ret;
    int i;

    ret = 0;
    i = 0;
    if(x)
        ret = ret_exit(x);
    printf("%u\n", ret);
    write(1,"exit\n",5);
    exit(ret);
}
int main(int ac, char **av)
{

    ft_exit(av[1]);    
    exit(0);
}