#ifndef FRIIMGUI_TYPES_HPP
#define FRIIMGUI_TYPES_HPP

#include <imgui/imgui.h>

#include <optional>

namespace friimgui {

struct Transformation {
private:
    template<typename T>
    class TransformationComponent {
    public:
        using ValueOpt = std::optional<T>;
        using PivotOpt = std::optional<ImVec2>;

    public:
        TransformationComponent() = default;

        TransformationComponent(const T &val) : m_valueOpt(val) {
        }

        TransformationComponent(const T &val, const ImVec2 &pivot) :
            m_valueOpt(val),
            m_pivotOpt(pivot) {
        }

        T getValueOrDef(const T &def = T()) const {
            return m_valueOpt.value_or(def);
        }

        ValueOpt getValueOpt() const {
            return m_valueOpt;
        }

        bool isValueDefined() const {
            return m_valueOpt.has_value();
        }

        void setValue(T val) {
            m_valueOpt = val;
        }

        void addValue(T val) {
            m_valueOpt = m_valueOpt.value_or(T()) + val;
        }

        void resetValue() {
            m_valueOpt = std::nullopt;
        }

        const ImVec2 getPivotOrDef(const ImVec2 &def = {0, 0}) const {
            return m_pivotOpt.value_or(def);
        }

        PivotOpt getPivotOpt() const {
            return m_pivotOpt;
        }

        bool isPivotDefined() const {
            return m_pivotOpt.has_value();
        }

        void setPivot(const ImVec2 &pivot) {
            m_pivotOpt = pivot;
        }

        void resetPivot() {
            m_pivotOpt = std::nullopt;
        }

    private:
        ValueOpt m_valueOpt = std::nullopt;
        PivotOpt m_pivotOpt = std::nullopt;
    };

public:
    using TranslationComponent = TransformationComponent<ImVec2>;
    using RotationComponent = TransformationComponent<float>;
    using ScaleComponent = TransformationComponent<float>;

public:
    Transformation() : translation(), rotation(0, {0, 0}), scale(1, {0, 0}) {
    }

    void reset() {
        translation.resetValue();
        translation.resetPivot();
        rotation.setValue(1);
        rotation.setPivot({0, 0});
        scale.setValue(1);
        scale.setPivot({0, 0});
    }

    TranslationComponent translation;
    RotationComponent rotation;
    ScaleComponent scale;
};

// ==================================================

class Region {
public:
    static Region createAtPosTopLeft(
        int posLeft,
        int posTop,
        int width,
        int height
    );
    static Region createAtPosTopLeft(int posLeft, int posTop, ImVec2 region);
    static Region createAtPosTopLeft(ImVec2 posTopLeft, ImVec2 region);

    static Region createAtPosCenter(
        int posCenterX,
        int posCenterY,
        int width,
        int height
    );
    static Region createAtPosCenter(
        int posCenterX,
        int posCenterY,
        ImVec2 region
    );
    static Region createAtPosCenter(ImVec2 posCenter, ImVec2 region);

    static Region createFromAvail();
    static Region createAtCursorScreenPos(int width, int height);
    static Region createAtCursorScreenPos(ImVec2 region);

    Region(const Region &other) = default;
    Region &operator= (const Region &other) = default;

    ImVec2 calculateCenter() const;
    ImVec2 calculateSize() const;
    void reserveSpace() const;

    const ImVec2 &getP0() const;
    const ImVec2 &getP1() const;
    const ImVec2 &getP2() const;
    const ImVec2 &getP3() const;

    Region translate(ImVec2 vec);
    Region translate(
        const Transformation::TranslationComponent &translationComponent
    );
    void translateThis(ImVec2 vec);
    void translateThis(
        const Transformation::TranslationComponent &translationComponent
    );

    Region rotate(float angleRad);
    Region rotate(float angleRad, const ImVec2 &pivot);
    Region rotate(const Transformation::RotationComponent &rotationComponent);
    void rotateThis(float angleRad);
    void rotateThis(float angleRad, const ImVec2 &pivot);
    void rotateThis(const Transformation::RotationComponent &rotationComponent);

    Region scale(float s);
    Region scale(float s, const ImVec2 &pivot);
    Region scale(const Transformation::ScaleComponent &scaleComponent);
    void scaleThis(float s);
    void scaleThis(float s, const ImVec2 &pivot);
    void scaleThis(const Transformation::ScaleComponent &scaleComponent);

private:
    Region(ImVec2 topLeft, ImVec2 bottomRight);
    Region(ImVec2 p0, ImVec2 p1, ImVec2 p2, ImVec2 p3);

    ImVec2 m_points[4];
};

template<>
inline void Transformation::TransformationComponent<ImVec2>::addValue(
    ImVec2 val
) {
    ImVec2 t = m_valueOpt.value_or(ImVec2 {0, 0});
    m_valueOpt = {t.x + val.x, t.y + val.y};
}

} // namespace friimgui

#endif
