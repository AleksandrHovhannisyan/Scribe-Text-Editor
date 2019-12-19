#include "settings.h"


/* Returns the singleton Settings, to which all parts of the
 * app have access.
 */
Settings *Settings::instance()
{
    static Settings singleton;
    return &singleton;
}


/* Simple wrapper for QSettings's setValue.
 */
void Settings::setValue(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}


/* Wrapper for QSettings's value.
 */
QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    return settings.value(key, defaultValue);
}


/* Applies the given setting using the provided handler function. Assumes
 * that the handler captures the caller's context (the this pointer).
 */
void Settings::apply(QVariant setting, std::function<void(QVariant)> handler)
{
    if (!setting.isNull())
    {
        handler(setting);
    }
}
