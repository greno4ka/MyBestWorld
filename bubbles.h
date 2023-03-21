#ifndef BUBBLES_H
#define BUBBLES_H

class Bubbles
{
    int numberOfBubbles;
    double maxHeight;
    double movieVar = 0;
    double **bubbles; // so many bubbles=))
public:
    Bubbles(int numOfBubbles, double maxH);

    void moveBubbles();
};

#endif // BUBBLES_H
