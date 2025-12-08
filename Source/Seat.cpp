#include "../Header/glad/glad.h"
#include "../Header/Seat.h"

Seat::Seat(float x, float y, float size)
    : x(x), y(y), size(size), status(SeatStatus::Free) {}

bool Seat::contains(float mx, float my) const
{
    float half = size;

    return (
        mx >= x - half && mx <= x + half &&
        my >= y - half && my <= y + half
    );
}


void Seat::toggleReserve()
{
    if (status == SeatStatus::Free)
        status = SeatStatus::Reserved;
    else if (status == SeatStatus::Reserved)
        status = SeatStatus::Free;
}

void Seat::setBought()
{
    status = SeatStatus::Bought;
}

void Seat::draw(unsigned int shader, unsigned int vao)
{
    glUseProgram(shader);

    float r=0, g=0, b=0;

    switch (status) {
        case SeatStatus::Free:     r=0.2f; g=0.4f; b=1.0f; break;
        case SeatStatus::Reserved: r=1.0f; g=0.9f; b=0.2f; break;
        case SeatStatus::Bought:   r=1.0f; g=0.1f; b=0.1f; break;
    }

    glUniform3f(glGetUniformLocation(shader, "uColor"), r, g, b);
    glUniform2f(glGetUniformLocation(shader, "uPos"), x, y);
    glUniform1f(glGetUniformLocation(shader, "uSize"), size);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
