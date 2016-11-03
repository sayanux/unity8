import QtQuick 2.4
import Ubuntu.Components 1.3
import QtGraphicalEffects 1.0

Item {
    id: root

    property int blurAmount: 32
    property Item sourceItem
    property rect blurRect: Qt.rect(0,0,0,0)
    property alias cached: fastBlur.cached

    Item {
        anchors.fill: parent
        Rectangle {
            id: blurMask
            color: "yellow"
            x: blurRect.x
            y: blurRect.y
            width: blurRect.width
            height: blurRect.height
        }
    }

    ShaderEffect {
        id: maskedBlurEffect
        x: blurRect.x
        y: blurRect.y
        width: blurRect.width
        height: blurRect.height

        property variant source: ShaderEffectSource {
            id: shaderEffectSource
            sourceItem: root.sourceItem
            hideSource: false
            sourceRect: root.blurRect
        }

        property var mask: ShaderEffectSource {
            sourceItem: blurMask
            hideSource: true
        }

        fragmentShader: "
            varying highp vec2 qt_TexCoord0;
            uniform sampler2D source;
            uniform sampler2D mask;
            void main(void)
            {
                highp vec4 sourceColor = texture2D(source, qt_TexCoord0);
                highp vec4 maskColor = texture2D(mask, qt_TexCoord0);

                sourceColor *= maskColor.a;

                gl_FragColor = sourceColor;
            }"
    }

    FastBlur {
        id: fastBlur
        x: blurRect.x
        y: blurRect.y
        width: blurRect.width
        height: blurRect.height
        source: maskedBlurEffect
        radius: Math.min(blurAmount, 128)
    }
 }
