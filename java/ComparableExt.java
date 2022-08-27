package java;

public interface ComparableExt<T> extends Comparable<T>
{
    default boolean lt(T other)
    {
        return this.compareTo(other) < 0;
    }

    default boolean leq(T other)
    {
        return this.compareTo(other) <= 0;
    }

    default boolean gt(T other)
    {
        return this.compareTo(other) > 0;
    }

    default boolean geq(T other)
    {
        return this.compareTo(other) >= 0;
    }
}
