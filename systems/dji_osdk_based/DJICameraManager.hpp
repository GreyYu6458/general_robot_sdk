#ifndef _DJI_CAMERA_INFO_HPP_
#define _DJI_CAMERA_INFO_HPP_
#include <vector>
#include <unordered_set>
#include <string>

class DJIVehicleSystem;

/**
 * @brief 文件类型，目前只记录JPG和其他
 * 
 */
enum class DJIFileType
{
    JPG,
    OTHERS
};

/**
 * @brief
 * 
 */
struct DJIMediaFile
{
    std::string     name;
    int             index;
    DJIFileType     type;
};

struct DJIMediaHash
{
    size_t operator()(const DJIMediaFile& file) const
    {
        return std::hash<std::string>()(file.name);
    }
};

struct DJIMediaEqual
{
    bool operator()(const DJIMediaFile& file1, const DJIMediaFile& file2) const
    {
        return file1.index == file2.index;
    }
};

using DJIMediaSet = std::unordered_set<DJIMediaFile, DJIMediaHash, DJIMediaEqual>;

class DJICameraManager
{
public:
    explicit DJICameraManager(DJIVehicleSystem*);

    ~DJICameraManager();

    bool initization();

    // 云台相机是否挂载
    bool isMainCameraEnable();

    // 云台相机中文件名称的集合
    DJIMediaSet& cameraFileSet();

    using SyncRst = std::pair<std::vector<const DJIMediaFile*>, bool>;
    
    /**
     * @brief 更新云台数据
     * 
     * @return SyncRst 
     */
    SyncRst updateFilesSetSync();

private:
    class Impl;
    Impl* _impl;
};

#endif
