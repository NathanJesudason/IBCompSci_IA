template <int N> class MovingAverage
{
public:
  
    void StartAvg(){
      sum = 0;
      p = 0;
        for (int i = 0; i < N; i++) {
            data[i] = 0;
            sum += data[i];
        }
    }
    
  
    int add(int new_sample)
    {   
        sum = sum - data[p] + new_sample;
        data[p] = new_sample;
        if (p >= N){
            p = 0;
        }
        else {
        p += 1;
        }
        return sum / N;
    }
    
private:
    int data[N];
    int sum;
    int p;
};
