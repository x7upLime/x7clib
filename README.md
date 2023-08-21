# x7clib
my personal library for all the interesting things in C

Finally following [Andreas Kling](https://awesomekling.github.io/)'s [wonderful advice](https://www.youtube.com/watch?v=XpzPT_9qzPI&t=342s)
and making my own personal c library.  
It feels like a perfect place to put all the interesting pieces of code that I encounter or things that I try to
implement, instead of writing them once and forgetting about them.

##### my Adventure:
+ did a coding interview in golang and failed (not completely bombed ..but you could tell I was missing something),
  that is because I don't code regularly, even tho when I do it, I enjoy it.
+ I started completing a couple of challenges on leetcode in golang.
+ I wanted to learn data structures and algorithms (I've been avoiding them for too much time).
+ I wanted to write a quicksort algorithm (started [here](https://cboard.cprogramming.com/c-programming/177660-quick-sort.html))
  and wrote a demo.c for it.   
  Then since I was cat-on-the-keyboard-typing to define the array values, I wanted to
  display them in order and count how many times each one occurs.. to know if my quicksort was doing ok.  
  So I wrote this:
  ```C
  int
  countOccur(int arr[],int exists[], int len, int val) {
    int i;
    int cntr = 0;

    for(i = 0; i < len; i++) {
      if (arr[i] == val) {
        if (exists[i] != 0) return 0;
        cntr++;
      }
    }

    return cntr; 
  }

  int
  main(void){
    int miarr[] = {4,1,3,5,21,6,26,7,232,1,5,6,2,391,93,2,14,423,234,6,6,3,32,1,34,4,56,2,7,8,4,3,12,1,4,63};

    int r = (sizeof(miarr)/sizeof(miarr[0]));

    int c;
    int counts[r];
    memset(&counts, 0, r*sizeof(counts[0]));

    for (int i = 0; i < r; i++) {
      c = countOccur(miarr, counts, r, miarr[i]);
      printf("Got %4d %d times\n", miarr[i], c);
    }
  }
  ```

  But then I wanted a mechanism to store/display the numbers encountered,
  in a golang fashion:
  ```golang
  mp := make(map[int]int)

  for _, num := range nums {
    mp[num]++
  }
  ```
+ Theen I found [this wonderful article](https://benhoyt.com/writings/hash-table-in-c/) from [Ben Hoyt](https://benhoyt.com/)
+ Then I remembered... (and now I can close some tabs)


I started by talking to a machine, then I found one of **those** aricles/resources
[here](http://www.crasseux.com/books/ctutorial/Building-a-library.html#Building%20a%20library). From that point on
it was just a matter of `man ldconfig` and taking inspiration from [raylib](https://github.com/raysan5/raylib) for my makefile.
