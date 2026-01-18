import bpy
import bmesh


def write_bcf_data(filepath):
    # get selected object mesh
    print("Starting to write data-----------------")
    me = bpy.context.active_object.data
    bm = bmesh.new()
    bm.from_mesh(me)

    bmesh.ops.triangulate(bm, faces=bm.faces[:])
    # face count after triangulation
    triCount = len(bm.faces)
    bm.to_mesh(me)
    bm.free()
    
    print(f"Found {triCount} triangles")
    str_vertices = [f"{v.co[0]} {v.co[1]} {v.co[2]}" for v in me.vertices]
    print(f"Num vertices: {len(me.vertices)}")
    str_normals = [f"{n.normal[0]} {n.normal[1]} {n.normal[2]}" for n in me.vertices]
    f = open(filepath, "w", encoding='utf-8')
    f.write(f"{triCount} ")
    f.write(f"{' '.join(str_vertices)} {' '.join(str_normals)}")
    f.close()

    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportBCFData(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_test.some_data"  # Important since its how bpy.ops.import_test.some_data is constructed.
    bl_label = "Export mesh in BCF format"

    # ExportHelper mix-in class uses this.
    filename_ext = ".bcf"

    filter_glob: StringProperty(
        default="*.bcf",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    def execute(self, context):
        return write_bcf_data(self.filepath)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportBCFData.bl_idname, text="BCF Export Operator")


# Register and add to the "file selector" menu (required to use F3 search "Text Export Operator" for quick access).
def register():
    bpy.utils.register_class(ExportBCFData)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportBCFData)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # Test call.
    bpy.ops.export_test.some_data('INVOKE_DEFAULT')
