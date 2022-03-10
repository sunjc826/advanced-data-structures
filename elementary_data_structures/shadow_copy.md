**Proposition** The shadow copy will always be complete before the time the stack overflows.

**Proof** 
Denote
- $s_{max}$ as the maximum size of the current array
- $s$ as the number of elements in the stack
- $\alpha \geq \frac{1}{2}$ as the threshold at which we start the shadow copy. To be precise, the shadow copy starts when $s\geq \alpha s_{max}$.
- Let $n$ be the number of elements with each push.
- $\beta s_{max}$ as the capacity of the new array

Also note that each operation increases $s$ at most 1. (i.e. push increases $s$ by 1, pop even decreases s).

Then we give the following sequence of derivations.
- Copying starts at $s\geq \alpha s_{max} > s-1$, so that $s = \lceil \alpha s_{max} \rceil$
- The minimum number of operations before the stack overflows is $s_{max} - s$
- The number of elements to be copied is at most $s_{max}$.
- $n\geq \frac{\text{elements to copy}}{\text{number of steps}}$

Hence, in the worst case,
$$
    n \geq \frac{s_{max}}{s_{max} - s} \geq \frac{s_{max}}{s_{max} - \alpha s_{max}} = \frac{1}{1-\alpha}
$$

Since $n$ is a whole number, this is equivalent to $n\geq \lceil \frac{1}{1-\alpha} \rceil$ (see Concrete Mathematics ceilings and floors for a listing of such equivalences).

We are not yet done with the proof. We have shown that in a single iteration, assuming that copying always begins when $s = \lceil \alpha s_{max} \rceil$, $n\frac{1}{1-\alpha}$ is sufficient to fully copy the stack. However, this is only 1 iteration. Consider what happens when we move to a copy of the current stack. Suppose the copy of the current stack has size $s' > \lceil \alpha s'_{max}\rceil$, where $s' = s_{max}$ and $s'_{max} = \beta s_{max}$. Then we have a problem as our assumption when copying begins has failed.
Hence, a sufficient and necessary condition for multiple iterations to work properly is that $s' < \lceil \alpha s'_{max}\rceil$, or equivalently
$$
    s_{max} < \alpha \beta s_{max}
$$
We can remove the ceiling since $s_{max}$ is an integer.
This is equivalent to $\alpha \cdot \beta > 1$.

**Remark** Trivially, this also implies $\beta > 1$, since $\alpha < 1$.

In particular, when $\alpha = 0.75$, we have $\frac{1}{1-\alpha} = 4$. And indeed, we copy $4$ elements with each push operations.
If at any time, we copy strictly fewer than $4$ elements, this implies that the copy has the same number of elements as the stack. So this doesn't pose any issues. 
Also, $\beta = 2$, so that $\alpha \cdot \beta = 2\cdot 0.75 > 2\cdot 0.5 = 1$. This completes the proof.
$\square$
