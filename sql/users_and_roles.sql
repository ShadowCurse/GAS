create role gas_admin_role;
grant all on database gas to gas_admin_role;

create role gas_dev_role;
grant select,insert on resourcetype, resource, dependency, commits to gas_dev_role;
grant update on resourcetype, resource, dependency to gas_dev_role;

create user gas_admin with password 'admin_password';
grant gas_admin_role to gas_admin;

create user gas_dev with password 'password';
grant gas_dev_role to gas_dev;
