#include "../Header/SeatGrid.h"
#include <cmath>

SeatGrid::SeatGrid(int rows, int cols, float seatSize)
    : rows(rows), cols(cols), seatSize(seatSize)
{
    float spacing = seatSize * 2.5f;
    float totalW = cols * spacing;
    float totalH = rows * spacing;
    float startX = -totalW / 2.0f + seatSize;
    float startY =  totalH / 2.0f - seatSize;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {

            float x = startX + c * spacing;
            float y = startY - r * spacing;

            seats.emplace_back(x, y, seatSize);
        }
    }
}

void SeatGrid::draw(unsigned int shader, unsigned int vao)
{
    for (auto& s : seats)
        s.draw(shader, vao);
}

Seat* SeatGrid::getSeatAt(float mouseX, float mouseY)
{
    for (auto& s : seats)
        if (s.contains(mouseX, mouseY))
            return &s;
    return nullptr;
}

void SeatGrid::toggleSeat(float mouseX, float mouseY)
{
    Seat* s = getSeatAt(mouseX, mouseY);
    if (s && s->status != SeatStatus::Bought)
        s->toggleReserve();
}

std::vector<Seat*> SeatGrid::findContiguousFreeSeats(int count)
{
    for (int r = rows - 1; r >= 0; r--)
    {
        std::vector<Seat*> block;
        block.reserve(count);

        for (int c = cols - 1; c >= 0; c--)
        {
            Seat& s = seats[r * cols + c];

            if (s.status == SeatStatus::Free)
            {
                block.push_back(&s);
                if ((int)block.size() == count)
                    return block;
            }
            else
            {
                block.clear();
            }
        }
    }

    return {};
}


void SeatGrid::markBought(const std::vector<Seat*>& seatsToBuy)
{
    for (Seat* s : seatsToBuy)
        s->setBought();
}

int SeatGrid::getReservedCount()
{
    int cnt = 0;
    for (auto& s : seats)
        if (s.status == SeatStatus::Reserved)
            cnt++;
    return cnt;
}

int SeatGrid::getBoughtCount()
{
    int cnt = 0;
    for (auto& s : seats)
        if (s.status == SeatStatus::Bought)
            cnt++;
    return cnt;
}

void SeatGrid::resetSeats()
{
    for (auto& s : seats)
        s.status = SeatStatus::Free;
}

