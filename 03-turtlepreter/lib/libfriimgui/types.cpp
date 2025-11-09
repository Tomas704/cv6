#include "types.hpp"

#include <algorithm>
#include <cmath>

namespace friimgui {

Region Region::createAtPosTopLeft(
    int posLeft,
    int posTop,
    int width,
    int height
) {
    return {
        {static_cast<float>(posLeft),         static_cast<float>(posTop)},
        {static_cast<float>(posLeft + width),
         static_cast<float>(posTop + height)                            }
    };
}

Region Region::createAtPosTopLeft(int posLeft, int posTop, ImVec2 region) {
    return {
        {static_cast<float>(posLeft), static_cast<float>(posTop)},
        {posLeft + region.x,          posTop + region.y         }
    };
}

Region Region::createAtPosTopLeft(ImVec2 posTopLeft, ImVec2 region) {
    return {
        posTopLeft,
        {posTopLeft.x + region.x, posTopLeft.y + region.y}
    };
}

Region Region::createAtPosCenter(
    int posCenterX,
    int posCenterY,
    int width,
    int height
) {
    float w = static_cast<float>(width) / 2;
    float h = static_cast<float>(height) / 2;

    return {
        {posCenterX - w, posCenterY - h},
        {posCenterX + w, posCenterY + h}
    };
}

Region Region::createAtPosCenter(
    int posCenterX,
    int posCenterY,
    ImVec2 region
) {
    float w = region.x / 2;
    float h = region.y / 2;

    return {
        {posCenterX - w, posCenterY - h},
        {posCenterX + w, posCenterY + h}
    };
}

Region Region::createAtPosCenter(ImVec2 posCenter, ImVec2 region) {
    float w = region.x / 2;
    float h = region.y / 2;

    return {
        {posCenter.x - w, posCenter.y - h},
        {posCenter.x + w, posCenter.y + h}
    };
}

Region Region::createFromAvail() {
    return Region::createAtCursorScreenPos(ImGui::GetContentRegionAvail());
}

Region Region::createAtCursorScreenPos(int width, int height) {
    return Region::createAtCursorScreenPos(
        {static_cast<float>(width), static_cast<float>(height)}
    );
}

Region Region::createAtCursorScreenPos(ImVec2 region) {
    return Region::createAtPosTopLeft(ImGui::GetCursorScreenPos(), region);
}

ImVec2 Region::calculateCenter() const {
    float x = 0;
    float y = 0;
    for (int i = 0; i < 4; ++i) {
        x += m_points[i].x;
        y += m_points[i].y;
    }
    return {x / 4, y / 4};
}

ImVec2 Region::calculateSize() const {
    ImVec2 min = {FLT_MAX, FLT_MAX};
    ImVec2 max = {-FLT_MAX, -FLT_MAX};

    for (int i = 0; i < 4; i++) {
        min.x = std::min(min.x, m_points[i].x);
        min.y = std::min(min.y, m_points[i].y);
        max.x = std::max(max.x, m_points[i].x);
        max.y = std::max(max.y, m_points[i].y);
    }

    return {max.x - min.x, max.y - min.y};
}

void Region::reserveSpace() const {
    ImGui::Dummy(calculateSize());
}

const ImVec2 &Region::getP0() const {
    return m_points[0];
}

const ImVec2 &Region::getP1() const {
    return m_points[1];
}

const ImVec2 &Region::getP2() const {
    return m_points[2];
}

const ImVec2 &Region::getP3() const {
    return m_points[3];
}

Region Region::translate(ImVec2 vec) {
    Region result(*this);
    result.translateThis(vec);
    return result;
}

Region Region::translate(
    const Transformation::TranslationComponent &translationComponent
) {
    return Region::translate(translationComponent.getValueOrDef());
}

void Region::translateThis(ImVec2 vec) {
    for (int i = 0; i < 4; ++i) {
        m_points[i] = {m_points[i].x + vec.x, m_points[i].y + vec.y};
    }
}

void Region::translateThis(
    const Transformation::TranslationComponent &translationComponent
) {
    translateThis(translationComponent.getValueOrDef());
}

Region Region::rotate(float angleRad) {
    return rotate(angleRad, calculateCenter());
}

Region Region::rotate(float angleRad, const ImVec2 &pivot) {
    Region result(*this);
    result.rotateThis(angleRad, pivot);
    return result;
}

Region Region::rotate(
    const Transformation::RotationComponent &rotationComponent
) {
    return Region::rotate(
        rotationComponent.getValueOrDef(),
        rotationComponent.getPivotOrDef()
    );
}

void Region::rotateThis(float angleRad) {
    rotateThis(angleRad, calculateCenter());
}

void Region::rotateThis(float angleRad, const ImVec2 &pivot) {
    float s = sinf(angleRad);
    float c = cosf(angleRad);

    for (int i = 0; i < 4; ++i) {
        float x = m_points[i].x - pivot.x;
        float y = m_points[i].y - pivot.y;

        m_points[i] = {pivot.x + x * c - y * s, pivot.y + x * s + y * c};
    }
}

void Region::rotateThis(
    const Transformation::RotationComponent &translationComponent
) {
    rotateThis(
        translationComponent.getValueOrDef(),
        translationComponent.getPivotOrDef()
    );
}

Region Region::scale(float s) {
    return scale(s, calculateCenter());
}

Region Region::scale(float s, const ImVec2 &pivot) {
    Region result(*this);
    result.scaleThis(s, pivot);
    return result;
}

Region Region::scale(const Transformation::ScaleComponent &scaleComponent) {
    return Region::scale(
        scaleComponent.getValueOrDef(),
        scaleComponent.getPivotOrDef()
    );
}

void Region::scaleThis(float s) {
    scaleThis(s, calculateCenter());
}

void Region::scaleThis(float s, const ImVec2 &pivot) {
    for (int i = 0; i < 4; ++i) {
        m_points[i]
            = {pivot.x + (m_points[i].x - pivot.x) * s,
               pivot.y + (m_points[i].y - pivot.y) * s};
    }
}

void Region::scaleThis(const Transformation::ScaleComponent &scaleComponent) {
    scaleThis(scaleComponent.getValueOrDef(), scaleComponent.getPivotOrDef());
}

Region::Region(ImVec2 p0, ImVec2 p2) :
    Region(p0, {p2.x, p0.y}, p2, {p0.x, p2.y}) {
}

Region::Region(ImVec2 p0, ImVec2 p1, ImVec2 p2, ImVec2 p3) :
    m_points {p0, p1, p2, p3} {
}

} // namespace friimgui
