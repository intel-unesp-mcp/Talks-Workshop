void vec1(float *a, float *b, int off, int len)
{
  int i; 
  for(i = 0; i < len; i++)
  {
    a[i] = (sin(cos(a[i])) > 2.34) ? 
    a[i] * atan(b[i]) :
    a[i] + cos(sin(b[i]));
  }
}
  
void vec2(float *a, float *b, int off, int len)
{
  int i; 
  #pragma omp simd 
  for(i = 0; i < len; i++)
  {
    a[i] = (sin(cos(a[i])) > 2.34) ? 
    a[i] * atan(b[i]) :
    a[i] + cos(sin(b[i]));
  }
}


void vec3(float *a, float *b, int off, int len)
{
  int i; 
  
  #pragma omp simd aligned(a:64, b:64) simdlen(64)
  for(i = 0; i < len; i++)
  {
    a[i] = (sin(cos(a[i])) > 2.34) ? 
    a[i] * atan(b[i]) :
    a[i] + cos(sin(b[i]));
  }
}


void vec4(float *a, float *b, int off, int len)
{
  int i; 
  #pragma omp simd aligned(a:32, b:32) simdlen(64)
  for(i = 0; i < len; i++)
  {
    a[i] = (sin(cos(a[i])) > 2.34) ? 
    a[i] * atan(b[i]) :
    a[i] + cos(sin(b[i]));
  }
}
//
#pragma omp declare simd 
float func1(float a, float b)
{
  int k;
for(k = 0; k < 1000; k++) {

      a += (sin(cos(a)) > 2.34) ? 
      a * atan(b) :
      a + cos(sin(b));
}
     return(a);
}

#pragma omp declare simd 
float func2(float a, float b, float c, int off)
{
     a = a*off + b*(off/3) + c*(off/5);
     return(a);
}

void vec5(float *a, float *b, float *c, int off, int len)
{
  int i; 
  //simd aligned(a:64, b:64) simdlen(64)
  #pragma omp parallel for 
  for(i = 0; i < len; i++)
  {
    a[i] = func1(a[i],b[i]);
    b[i] = func1(b[i],c[i]);
    c[i] = func1(c[i],b[i]);

    a[i] = func2(a[i],b[i],c[i],off);
    b[i] = func2(b[i],a[i],c[i],off);
    c[i] = func2(c[i],b[i],a[i],off);
        off++;
    if (off == 1000 ) { off=1;}
  }

  /*  for(k = 0; k < 1000; k++) {

      a[i] += (sin(cos(a[i])) > 2.34) ? 
      a[i] * atan(b[i]) :
      a[i] + cos(sin(b[i]));

      b[i] += (sin(cos(b[i])) > 5.34) ? 
      b[i] * atan(c[i]) :
      b[i] + cos(sin(c[i]));

      c[i] += (sin(cos(c[i])) > 8.34) ? 
      c[i] * atan(b[i]) :
      c[i] + cos(sin(b[i]));

      a[i] = a[i]*off + b[i]*(off/3) + c[i]*(off/5);
      b[i] = c[i]*off + b[i]*(off/3) + a[i]*(off/5);
      c[i] = b[i]*off + a[i]*(off/3) + c[i]*(off/5);
    }
  */

}

void vec6(float *a, float *b, int off, int len)
{
	a[2] = 0;
	float sum = 0.0f;
	float *p ;// p = a[2];
	//float p = 1;
	int step = 2;
	int N = 10;
	int i = 0;
	p = &a[2];
	//reduction(+:sum) 
	printf("p %f %f \n", &p, a[2]);

    //#pragma omp simd //linear(p:step) 
	for (i = 0; i < N; ++i) {
		sum += *p;
		//sum += p;
		p += step;
		printf("p %f sum %f\n", &p, sum);
	}
	printf("sum %f\n", sum);
	/*
	int i;

#pragma omp simd aligned(a:64, b:64) simdlen(64)
	for (i = 0; i < len; i++)
	{
		a[i] = (sin(cos(a[i])) > 2.34) ?
			a[i] * atan(b[i]) :
			a[i] + cos(sin(b[i]));
	}*/

}

void vec7(float *a, float *b, int off, int len)
{
	int k;
	
	printf("off %d len %d\n", off, len);
    #pragma omp target data //to(a[0:len]) to(b[0:len])  map(off, len)
	{
		int y = 1;
		for (k = 0; k < 10; k++)
		{
		y = 1;
		#pragma omp task depend(out:y) 
		{ 	
			#pragma omp target update to(a[0:len] , b[0:len] ,k, len)
		}
		
		#pragma omp task depend(in:y) 
		{
			#pragma omp target //nowait
			{
			int i;
			#pragma omp simd aligned(a:64, b:64) simdlen(64)
			for (i = 0; i < len; i++)
			{
				a[i] = (a[i] > 2.34) ?
					a[i] * b[i] :
					a[i] + b[i];
				b[i] = (b[i] > 2.34) ?
					(b[i] * a[i]) + off :
					(b[i] + a[i])*off;

			}
		}
		}
		//#pragma omp target update from(a[0:len], b[0:len], off, len) nowait
		}
	}
}