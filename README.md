## Philosophers

This project focuses on solving the dining philosophers problem, this is often used as an example to illustrate synchronization in multiple threads and techniques for resolving them.

We have the philosophers sitting around the table, and each philosopher has one fork, but he needs 2 forks to eat.

So we have :

Philosopher 1 : needs forks 1 and 2 to start eating.

Philosopher 2 : needs forks 2 and 3 to start eating.

Philosopher 3 : needs forks 3 and 4 to start eating.

And so on...

After the philosophers eats, he has a time to start eating again, if he does'nt start eat after this he will die.

The problem is that, different from multiple processes, the variables in the threads are shared, so you have to "protect" it so you don't access it at the same time.

Once Philosopher 1 is eating he is using the 2 fork so Philosopher 2 can't start eating so he keeps "waiting".

So Philosopher 3 starts to eat after 1 because he does't need any forks that pilosopher 1 are using.

That's the thing, philosophers are threads and the mutex functions are the forks.

The mutex is designed to lock the forks, so no one else can use.

Another function used is usleep, because sometimes you have to wait for a little bit so your thread finishes.

## Using it

### Dependencies

To use the program you have to type 4 or 5 arguments.

◦ number_of_philosophers: The number of philosophers and also the number
of forks.

◦ time_to_die : If a philosopher didn’t start eating time_to_die

since the beginning of their last meal or the beginning of the simulation, they die.

◦ time_to_eat: The time it takes for a philosopher to eat.

During that time, they will need to hold two forks.

◦ time_to_sleep): The time a philosopher will spend sleeping.

◦ number_of_times_each_philosopher_must_eat (optional): If all

philosophers have eaten at least number_of_times_each_philosopher_must_eat

times, the simulation stops.

./philo [nbr_of_philosophers] [die_time] [eat_time] [sleep_time] [optional: number_of_times_each_philosopher_must_eat]

Run:

./philo 2 410 200 200 1

0 1 has taken a fork 

0 1 has taken a fork 

0 1 is eating 

200 1 is sleeping 

200 2 has taken a fork 

200 2 has taken a fork 

200 2 is eating 

400 1 is thinking 

401 2 is sleeping 

601 2 is thinking 

Simple overview of use/purpose.

### Installing

* How/where to download your program
* Any modifications needed to be made to files/folders

### Executing program

* How to run the program
* Step-by-step bullets
```
code blocks for commands
```

## Help

Any advise for common problems or issues.
```
command to run if program contains helper info
```

## Authors

Contributors names and contact info

ex. Dominique Pizzie  
ex. [@DomPizzie](https://twitter.com/dompizzie)

## Version History

* 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [awesome-readme](https://github.com/matiassingers/awesome-readme)
* [PurpleBooth](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2)
* [dbader](https://github.com/dbader/readme-template)
* [zenorocha](https://gist.github.com/zenorocha/4526327)
* [fvcproductions](https://gist.github.com/fvcproductions/1bfc2d4aecb01a834b46)
