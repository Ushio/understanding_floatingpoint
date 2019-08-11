__kernel void find_max(__global float *values, __global uint *max_value) {
    __local uint local_max;
    if(get_local_id(0) == 0) { 
        local_max = 0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    float value = values[get_global_id(0)];

    atomic_max(&local_max, as_uint(value));
    
    barrier(CLK_LOCAL_MEM_FENCE);
    if(get_local_id(0) == 0) { 
        atomic_max(max_value, local_max);
    }
}