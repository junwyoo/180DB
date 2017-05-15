#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "constants.h"
#include "classifiers.h"


int temp_strides(float **data, int *out, float *sigma_y, float *sigma_z, float max_accel,
                 float max_gyro, float max_time, float min_time, int n)
{
    float sum_ay, sum_gz, dif_time, dif_false, last, last_false;
    int i, j, cnt;
    sum_ay = 0;
    sum_gz = 0;
    cnt = 0;
    last = data[0][0];
    last_false = data[0][0];
//    printf("Strides start\n");
    for(i = 0; i < n; i++) {
        if (data[2][i] > sigma_y[i]) {
            sum_ay += data[2][i] - sigma_y[i];
//            printf("y");
        }
        if (data[6][i] < - 1.4 * sigma_z[i]) {
            sum_gz -= (data[6][i] + sigma_z[i]);
//            printf("g");
        }
        dif_time = data[0][i] - last;
        dif_false = data[0][i] - last_false;
        if (sum_gz > max_gyro || sum_ay > max_accel) {
            sum_ay = 0;
            sum_gz = 0;
            if (dif_time > max_time)
            {
//                printf("too long ");
                last = data[0][i];
                last_false = data[0][i];
            }
            else if(dif_false < min_time || sigma_y[i] < 0.05)
            {
//                printf("too short ");
                last_false = data[0][i];
            }
            else
            {
                last = data[0][i];
                last_false = data[0][i];
                out[cnt] = i;
                cnt++;
//                printf("Stride detected\n");
            }
        }
    }
    return cnt;
}

//def temp_strides(data, sigmas, max_accel, max_gyro, max_time, min_time = 0.22):
//    sum_acceleration = 0
//    sum_gyro = 0
//    last = data[0][0]
//    last_false = data[0][0]
//    out = []
//    temp = []
//    N = len(data[0])
//    # print(N)
//    for i in range(0, N):
//        # print(len(data[2][i]), len(sigmas[1][0]))
//        if data[2][i] > sigmas[1][i]:
//            sum_acceleration += data[2][i] - sigmas[1][i]
//        if data[6][i] < -sigmas[5][i]:
//            sum_gyro -= data[6][i] + sigmas[5][i]
//
//        dif_time = data[0][i] - last
//        dif_false = data[0][i] - last_false
//        if sum_gyro > max_gyro or sum_acceleration > max_accel:
//            tg = sum_gyro
//            sum_acceleration = 0
//            sum_gyro = 0
//
//            if dif_time > max_time:
//                last = data[0][i]
//                last_false = data[0][i]
//            elif dif_false < min_time or sigmas[1][i] < 0.05:
//                last_false = data[0][i]
//            elif data[6][i] > 2* sigmas[5][i]:
//                last_false = data[0][i]
//            else:
//                # print(tg)
//                out.append(i)
//                last = data[0][i]
//                last_false = data[0][i]
//
//    return out

void sample_mean(float * data, float * out, int chunk, int n)
{
    int step, rest, i, j, start, end;
    float temp_mean, weight;
    step = (int) n/chunk;
    rest = n - chunk * step;
    for( j = 0; j < step +1; j++)
    {
        temp_mean = 0;
        start = j * chunk;
        end = start + chunk;
        weight = chunk;
        if(j == step)
        {
            end = start + rest;
            weight = rest;
        }
        temp_mean = 0;
        for(i = start; i < end; i ++)
        {
            temp_mean += data[i];
        }
//        printf("chunk: %d\n", start);
        temp_mean = temp_mean / weight;
        for(i = start; i < end; i++)
        {
            out[i] = temp_mean;
//            printf("mean: %f\n", out[i]);
        }
//        printf("temp_mean: %f\n", temp_mean);
    }
}


int select_strides(float * times, int * strides, int * out, float min_time, int n)
{
    int i, count, j;
    count = 0;
    float prev_time, current_time;
    if(n < 1)
        return 0;
    out[count] = strides[0];
    count++;

    for(i = 1; i < n; i++)
    {
        current_time = times[strides[i]];
        prev_time = times[strides[i-1]];
        if(current_time - prev_time > min_time)
        {
            out[count] = strides[i];
            count++;
        }
    }
    return count;
}


void sample_sigma(float * data, float * mean, float * out, int chunk, int n)
{
    int step, rest, i, j, start, end;
    float temp_square, weight;
    step = (int) n/chunk;
    rest = n - chunk * step;
    for( j = 0; j < step +1; j++)
    {
        start = j * chunk;
        end = start + chunk;
        weight = chunk;
        if(j == step)
        {
            end = start + rest;
            weight = rest;
        }
        temp_square = 0;
        for(i = start; i < end; i ++)
        {
            temp_square += (float) pow(data[i]-mean[i], 2);
        }
        temp_square = temp_square / weight;
        temp_square = (float) sqrt(temp_square);
        for(i = start; i < end; i++)
        {
            out[i] = temp_square;
        }
    }
}

//def sample_sigma_lh(data, mean, chunk = 3000):
//    N = len(data)
//    rest = N % chunk
//    quot = N // chunk
//    sigma = np.zeros(shape=N, dtype='float')
//    for k in range(quot + 1):
//        upper_sigma = 0
//        start = k * chunk
//        end = start + chunk
//        total = chunk
//        if k == quot:
//            end = start + rest
//            total = rest
//        for i in range(start, end):
//            temp = data[i] - mean[i]
//            upper_sigma += temp * temp
//        sigma[start: end] = upper_sigma / total
//    sigma = np.sqrt(sigma)
//    return sigma


void sample_sigma_2(float * data, float * mean, float ** out, int chunk, int n)
{

    int step, rest, i, j, start, end;
    float temp_square_up, temp_square_down, weight;
    int up, down;
    step = (int) n/chunk;
    rest = n - chunk * step;
    for( j = 0; j < step +1; j++)
    {
        start = j * chunk;
        end = start + chunk;
        if(j == step)
        {
            end = start + rest;
        }
        temp_square_up = temp_square_down = 0;
        up = down = 0;
        for(i = start; i < end; i ++)
        {
            if(data[i] < 0){
                temp_square_down += (float) pow(data[i]-mean[i], 2);
                down ++;
            }
            else{
                temp_square_up += (float) pow(data[i]-mean[i], 2);
                up ++;
            }


        }
        temp_square_up = temp_square_up / up;
        temp_square_up = (float) sqrt(temp_square_up);
        temp_square_down = temp_square_down / down;
        temp_square_down = (float) sqrt(temp_square_down);
        for(i = start; i < end; i++)
        {
            out[0][i] = temp_square_up;
            out[1][i] = temp_square_down;
        }
    }

}


void center_data(float * data, float * mean, float * sigma, int chunk, int n)
{
    int step, rest, i, j, start, end, ratio;
    float temp_center, weight;
    step = n / chunk;
    rest = n - chunk * step;
    for( j = 0; j < step +1; j++)
    {
        temp_center = 0;
        start = j * chunk;
        end = start + chunk;
        weight = chunk;
        if(j == step)
        {
            end = start + rest;
            weight = rest;
        }
        ratio = 1;
        for(i = start; i < end; i ++)
        {
            if(mean[i] - sigma[i] < data[i] && mean[i] + sigma[i] > data[i]){
                ratio ++;
                temp_center += data[i];
            }


        }
        temp_center = temp_center / ratio;
        for(i = start; i < end; i++)
        {
            data[i] -= temp_center;
        }
    }

}

//
//def find_center(data, sigma, mean, chunk = 3000, weight=1.5):
//    N = len(data)
//    rest = N % chunk
//    quot = N // chunk
//    out = np.zeros(shape=N, dtype='float')
//    for k in range(quot):
//        temp_center = 0
//        for i in range(k * chunk, (k + 1) * chunk):
//            if mean[i] - weight*sigma[1][i] < data[i] < mean[i] + weight * sigma[0][i]:
//                temp_center += data[i]
//        out[k * chunk: (k + 1) * chunk] = temp_center / chunk
//    temp_center = 0
//    for i in range(quot * chunk, quot * chunk + rest):
//        # print(len(mean), len(sigma), len(data))
//        if mean[i] - sigma[1][i] < data[i] < mean[i] + sigma[0][i]:
//            temp_center += data[i]
//    out[(quot) * chunk: (quot) * chunk + rest] = temp_center / rest
//    return out

//void center_data(float * data, float * center, int n)
//{
//    int i;
//    for(i = 0; i < n; i++)
//    {
//        data[i] -= center[i];
//    }
//}

int peak_strides(float * time, float * data, int * strides, int *holder, float * sigma, float weight, int sn, int n)
{
    int start, last_s, i , j, count, count2, t, last_end;
    float d1, d2, d3, d4;
    count = 0;
    count2 = 0;
    t = 0;
    last_end = 0;
//    printf("%d\n", sn);
//    printf("\npeaks:\n");
    for(j = 0; j < sn; j++)
    {
        start = strides[j] - 280;
        last_s = start + 560;
//        start = strides[j] - 150;
//        last_s = start + 300;
        if(start < 0)
        {
            start = 0;
        }
        if(start < last_end)
            start = last_end;
        if(last_s > n)
        {
            last_s = n;
        }
        last_end = last_s;
//        printf("%d, " , j);
//        printf("start: %d\t stride: %d\t last_s: %d\n", start, strides[j], last_s);
        for(i = start+1; i < last_s; i++)
        {

            if(- weight* sigma[i] > data[i] && data[i] > data[i-1])
            {
                t++;
//                if(j == sn - 1)
//                    printf("evaluating last: %d, last stride: %d, i: %d\n", t, strides[j], i);

                holder[count] = i;
                count++;
                break;
            }
        }
    }
//    printf("\nholder: %d\n", count);
    for(i = 0; i < count; i++)
        strides[i] = holder[i];


    return count;
}

void move_back(int * strides, int offset, int n)
{
    int i, start;
    start = 0;
    if(strides[0]-offset < 0)
    {
        strides[0] = 0;
        start = 1;
    }
    for(i = start; i < n; i++)
        strides[i] -= offset;
}

void sigma_stride(float * data, int ** stride, float * level, float * sigma_low, float * sigma_high, int n)
{
    double low, high;
    double n_low, n_high;
    int i, j;
    for(j = 0; j < n; j++)
    {
        low = 0;
        high = 0;
        n_low = 0;
        n_high = 0;
        for(i = stride[j][0]; i < stride[j][1]; i++)
        {
            if(data[i] < -level[i] || data[i] > level[i])
            {
//                printf("data: %f", data[i]);
                high += data[i] * data[i];
                n_high++;
            }
            else
            {
                low += data[i] * data[i];
                n_low++;
            }
        }
        if(n_low == 0)
            n_low = 1;
        if(n_high == 0)
            n_high = 1;
        low = low/n_low;
        high = high/n_high;
        high = sqrt(high);
        low = sqrt(low);
//        printf("high: %f, low: %f\n", n_high, n_low);
        sigma_low[j] = (float) low;
        sigma_high[j] = (float) high;
    }
}


void peaks(float * data, int ** strides, float ** max_peak, float **max_trough, int n)
{
    int i, j, h_1, h_2, l_1, l_2, max_1, min_1;
    float p_1, p_2, t_1, t_2;

    for(j = 0; j < n; j++)
    {
        p_1 = 0; p_2 = 0; t_1 = 0; t_2 = 0;
        max_1 = 0; min_1 = 0;
        h_1 = h_2 = l_1 = l_2 = strides[j][0];
        // from start of stride to en of stride
        for (i = strides[j][0]; i < strides[j][1]; i++) {
            if (data[i] > p_1) {
                p_1 = data[i];
                h_1 = i;
            }
            else if(data[i] < t_1) {
                t_1 = data[i];
                l_1 = i;
            }
        }
        for (i = strides[j][0]; i < strides[j][1]; i++) {
            if (data[i] > p_2 && (i < h_1  - 50 || i > h_1 + 50) ) {
                p_2 = data[i];
                h_2 = i;
            }
            else if(data[i] < t_2 && (i < l_1 - 50 || i > l_1 + 50) ) {
                t_2 = data[i];
                l_2 = i;
            }
        }
//        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t",strides[j][0],strides[j][1],strides[j][2], h_1, h_2, l_1, l_2);
        h_1 = h_1 - strides[j][0];
        h_2 = h_2 - strides[j][0];
        l_1 = l_1 - strides[j][0];
        l_2 = l_2 - strides[j][0];
        max_peak[j][0] = p_1;
        max_peak[j][1] = h_1;
        max_peak[j][2] = p_2;
        max_peak[j][3] = h_2;
        max_trough[j][0] = t_1;
        max_trough[j][1] = l_1;
        max_trough[j][2] = t_2;
        max_trough[j][3] = l_2;
//        printf("%f\t%d\t%f\t%d\t%f\t%d\t%f\t%d\n",p_1, h_1, p_2, h_2, t_1, l_1, t_2, l_2);

    }
}

void mean_slot(float * data, int ** strides, float * level, float ** mean_low, float ** mean_high, float slots, int n)
{
//    printf("new file\n");
    float low, high, n_h, n_l;
    int i, j, k, l;
    low = 0;
    high = 0;

    for(j = 0; j < n; j++)
    {
        l = (int) ((strides[j][1] - strides[j][0])/slots);
//        printf("l: %d\t", l);
        for(k = 0; k < slots; k++) {
            n_h = 0;
            n_l = 0;
            high = 0;
            low = 0;

            for (i = strides[j][0] + k * l; i < strides[j][0] + (k + 1) * l; i++) {
                if (data[i] > 0) {
                    high += data[i];
                    n_h++;
                }
                else if(data[i] < 0) {
                    low += data[i];
                    n_l++;
                }
            }
            if(n_l == 0)
                n_l = 1;
            if(n_h == 0)
                n_h = 1;

            mean_low[j][k] = low/n_l;
            mean_high[j][k] = high/n_h;
//            printf("from: %d\t to %d\thigh: %f\tlow:%f\n", strides[j][0] + k * l,
//                   strides[j][0] + (k + 1) * l, mean_low[j][k], mean_high[j][k]);
        }
    }
}


int outlier(float * data, int * strides, int * out, float min_diff, int n)
{
//    printf("outlier %d\n", n);
    float d1, d2, d3, d4, d5;
    int cnt, i;
    cnt = 0;
//    printf("here");
    if(n > 2)
    {
        out[cnt++] = strides[0];
        out[cnt++] = strides[1];
    }
//    printf("here\n");
    for(i = 2; i < n-2; i ++)
    {
        d1 = data[strides[i-2]];
        d2 = data[strides[i-1]];
        d3 = data[strides[i]];
        d4 = data[strides[i+1]];
        d5 = data[strides[i+2]];
        if((d3 - d2)/(d2 - d1) < min_diff && (d4 - d3)/(d5 - d4) < min_diff && (d2 - d1) < 4 && (d5 - d4) < 4)
            continue;
        out[cnt++] = strides[i];
    }
    if(n > 3)
    {
        out[cnt++] = strides[n-2];
        out[cnt++] = strides[n-1];
    }
    else if(n > 2)
        out[cnt++] = strides[n-1];
    return cnt;
}

//def outlier(data, strides, max_time):
//    out = []
//    if len(strides) > 2:
//        out.append(strides[0])
//        out.append(strides[1])
//    for i in range(2, len(strides) -2):
//        d1 = data[0][strides[i-2]]
//        d2 = data[0][strides[i-1]]
//        d3 = data[0][strides[i]]
//        d4 = data[0][strides[i+1]]
//        d5 = data[0][strides[i+2]]
//        if (d3 - d2)/(d2 - d1) < 0.6 and (d4 - d3)/(d5 - d4) < 0.6 and (d2- d1) < 3 and (d5 - d4) < 3:
//            continue
//        else:
//            out.append(strides[i])
//    if len(strides) > 3:
//        out.append(strides[-2])
//        out.append(strides[-1])
//    elif len(strides) > 2:
//        out.append(strides[2])
//    return out
//
