public class hw1{

     public static int fl(int [] x, int y)
     {
         for(int i = x.length-1; i>=0; i--)
         {
             if(x[i] == y)
             {
                 return i;
             }
         }
         return -1;
     }
     
     public static void main(String []args){
        
        int []xy = {2,3,5};
        int yz = 2;
        
        System.out.println(fl(xy,yz));
     
     }
}