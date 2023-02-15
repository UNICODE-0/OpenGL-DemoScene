mat4 translate_matrix(float x, float y, float z)
{ 
    return mat4
    (
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,                          
        x,   y,   z,   1.0
    );
}

mat4 rotateX_matrix(float rad)
{ 
    return mat4
    (
        1.0,    0.0,        0.0,        0.0,
        0.0,    cos(rad),   -sin(rad),  0.0,
        0.0,    sin(rad),   cos(rad),   0.0,                          
        0.0,    0.0,        0.0,        1.0
    );
}

mat4 rotateY_matrix(float rad)
{ 
    return mat4
    (
        cos(rad),   0.0,   sin(rad),    0.0,
        0.0,        1.0,   0.0,         0.0,
        -sin(rad),  0.0,   cos(rad),    0.0,                          
        0.0,        0.0,   0.0,         1.0
    );
}

mat4 rotateZ_matrix(float rad)
{ 
    return mat4
    (
        cos(rad),   -sin(rad),   0.0,    0.0,
        sin(rad),   cos(rad),    0.0,    0.0,
        0.0,        0.0,         1.0,    0.0,                          
        0.0,        0.0,         0.0,    1.0
    );
}

mat4 scale_matrix(float x, float y, float z)
{ 
    return mat4
    (
        x,   0.0, 0.0, 0.0,
        0.0, y,   0.0, 0.0,
        0.0, 0.0, z,   0.0,                          
        0.0, 0.0, 0.0, 1.0
    );
}