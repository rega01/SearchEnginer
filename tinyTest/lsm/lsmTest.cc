#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

// 模拟的 Memtable (内存表)
class Memtable {
public:
    void put(const std::string &key, const std::string &value) {
        table[key] = value;
    }

    std::string get(const std::string &key) const {
        auto it = table.find(key);
        if (it != table.end()) {
            return it->second;
        } else {
            return "";
        }
    }

    bool contains(const std::string &key) const {
        return table.find(key) != table.end();
    }

    void clear() {
        table.clear();
    }

    std::map<std::string, std::string> getTable() const {
        return table;
    }

private:
    std::map<std::string, std::string> table;  // 使用std::map模拟有序存储
};

// 模拟的 SSTable (磁盘存储)
class SSTable {
public:
    SSTable(const std::string &filename) : fileName(filename) {}

    void saveToDisk(const std::map<std::string, std::string> &memtableData) {
        std::ofstream file(fileName);
        for (const auto &entry : memtableData) {
            file << entry.first << " " << entry.second << std::endl;
        }
        file.close();
    }

    std::string get(const std::string &key) const {
        std::ifstream file(fileName);
        std::string fileKey, fileValue;

        while (file >> fileKey >> fileValue) {
            if (fileKey == key) {
                return fileValue;
            }
        }

        return "";
    }

private:
    std::string fileName;  // 存储文件名
};

// LSM Tree 简化实现
class LSMTree {
public:
    LSMTree(size_t threshold = 4)
        : memtable()
          ,sstable("sstable.txt")
          ,threshold(threshold)
    {}

    // 插入数据
    void put(const std::string &key, const std::string &value) {
        memtable.put(key, value);
        if (memtable.getTable().size() >= threshold) {
            flush();
        }
    }

    // 查询数据
    std::string get(const std::string &key) {
        if (memtable.contains(key)) {
            return memtable.get(key);
        }
        return sstable.get(key);
    }

private:
    Memtable memtable;      // 内存表
    SSTable sstable;        // 模拟的磁盘存储
    size_t threshold;       // Memtable 阈值（达到此值则 flush）

    // 将 Memtable 中的数据刷入 SSTable
    void flush() {
        std::cout << "Flushing Memtable to disk..." << std::endl;
        sstable.saveToDisk(memtable.getTable());
        memtable.clear();
    }
};

// 测试 LSM Tree
int main() {
    LSMTree lsmTree(3);  // 设置阈值为4，即 memtable 大小达到4时 flush 到磁盘

    lsmTree.put("name", "Alice");
    lsmTree.put("gender", "women");
    lsmTree.put("age", "60");
    /* lsmTree.put("city", "New York"); */
    lsmTree.put("job", "Engineer");

    // 此时已经达到阈值，Memtable 刷盘

    /* lsmTree.put("company", "Tech Corp");  // 触发 Memtable 刷盘操作 */

    std::cout << "Get name: " << lsmTree.get("name") << std::endl;    // 从磁盘读取
    /* std::cout << "Get city: " << lsmTree.get("city") << std::endl;    // 从磁盘读取 */
    std::cout << "Get job: " << lsmTree.get("job") << std::endl;      // 从磁盘读取
    std::cout << "Get company: " << lsmTree.get("gender") << std::endl;  // 在 Memtable 中找到

    return 0;
}

