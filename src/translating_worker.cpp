#include "translating_worker.h"

unsigned long translating_worker::get_freshness() const { return freshness; }

void translating_worker::set_freshness(unsigned long value) {
    freshness = value;
}

void translating_worker::set_text(const QString &text) { this->text = text; }

void translating_worker::set_path(const QString &path) { this->path = path; }
