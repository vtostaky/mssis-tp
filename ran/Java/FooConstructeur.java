import java.lang.*;

public class FooConstructeur{
    static int i;
    final static int MAX = 99;
    public FooConstructeur(int i){
        this.i = i;
    }
    public FooConstructeur(){this(2);} // j'appelle le constructeur FooConstructeur(int)

    public FooConstructeur(float a){super();} // j'appelle le constructeur parent
    
    void print(){
        System.out.println(i);
    }

    public static void main (String[] args){
        FooConstructeur c1 = new FooConstructeur(1);
        FooConstructeur c2 = new FooConstructeur(2);
        c1.print(); // va retoruner 2 car i est une variable statique
    }
}

