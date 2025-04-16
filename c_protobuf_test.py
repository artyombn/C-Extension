import c_protobuf

print(f"Writing to file\n'idfa', '123abc', 55.75, 37.61, [1, 2, 3, 4, 5]")

c_protobuf.save_to_protobuf(
    "idfa", "123abc", 55.75, 37.61, [1, 2, 3, 4, 5]
)

print(f"Writing done")
print(f"------------------------------")
print(f"Load writen file")
data = c_protobuf.load_from_protobuf("idfa_123abc.pb")
print(f"Data: {data}")